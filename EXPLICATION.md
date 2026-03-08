# NanoTekSpice — Explication complète du projet

## Vue d'ensemble

NanoTekSpice simule des circuits électroniques logiques. On lui donne un fichier `.nts`
qui décrit des composants (puces) et leurs connexions, et on peut ensuite simuler le
circuit tick par tick via un shell interactif.

---

## 1. La logique tristate (`include/circuit/tristate.hpp`)

Tout signal dans ce projet a trois états possibles :

```
Tristate::True      → 1 (courant)
Tristate::False     → 0 (pas de courant)
Tristate::Undefined → U (inconnu / non connecté)
```

C'est la base de tout. Chaque pin de chaque composant retourne un `Tristate`.

---

## 2. L'interface `IComponent` (`include/circuit/i_component.hpp`)

C'est la classe mère abstraite. Tous les composants en héritent.

```cpp
virtual Tristate compute(std::size_t pin) = 0;   // calculer la valeur d'un pin
virtual void set_link(...) = 0;                   // connecter un pin à un autre composant
virtual void simulate() = 0;                      // passer au tick suivant
virtual const std::string &get_name() const = 0; // nom du composant
```

Elle contient aussi `compute_safe()` — une méthode non-virtuelle qui appelle `compute()`
mais avec un garde anti-boucle infinie (`std::set<std::size_t> _computing_pins`). Si un
**pin spécifique** est déjà en cours de calcul et qu'on le redemande, il retourne `Undefined`
au lieu de boucler. Le suivi est **par pin** et non par composant, ce qui permet à plusieurs
portes indépendantes d'une même puce (ex: 4081) de se calculer sans se bloquer mutuellement.

---

## 3. La classe abstraite `AComponent` (`include/circuit/components/a_component.hpp`)

Implémente les parties communes à presque tous les composants :

- `_name` : le nom du composant (ex: `"a"`, `"clk"`)
- `_links` : une `map<pin, {composant*, pin_distant}>` — les connexions
- `get_name()` : retourne `_name`
- `simulate()` : ne fait rien par défaut (overridé si besoin)

Tous les composants concrets héritent de `AComponent`.

---

## 4. Les composants

### `InputComponent` (`input`)
- Un seul pin (pin 1) : sa valeur courante `_current`
- `set_value()` place une valeur dans `_pending`
- `simulate()` applique `_pending` → `_current`, puis efface le pending
- Sans `set_value()`, la valeur reste inchangée au prochain tick
- **Rôle** : entrée manuelle, l'utilisateur la change via le shell (`a=1`)

### `ClockComponent` (`clock`)
- Identique à `InputComponent` MAIS dans `simulate()` :
  - Si pending → applique le pending
  - Sinon → bascule automatiquement (`1→0` ou `0→1`)
- Reste `Undefined` tant qu'aucune valeur initiale n'est fournie
- **Rôle** : signal d'horloge, alterne seul à chaque tick

### `OutputComponent` (`output`)
- Un seul pin (pin 1) : **ne stocke rien**, recalcule à chaque fois
- `compute(1)` suit le lien vers le composant source et appelle `compute_safe()`
- `get_value()` = `compute(1)`
- **Rôle** : afficher le résultat du circuit

### `TrueComponent` (`true`)
- Retourne toujours `Tristate::True` sur le pin 1
- **Rôle** : source constante à 1

### `FalseComponent` (`false`)
- Retourne toujours `Tristate::False` sur le pin 1
- **Rôle** : source constante à 0

### `ElementaryGate` (`and`, `or`, `xor`)
- 3 pins : pin 1 = entrée A, pin 2 = entrée B, pin 3 = sortie
- `compute(3)` : récupère les valeurs des pins 1 et 2 via leurs liens, applique l'opération
- L'opération est une `std::function` passée au constructeur (`and_op`, `or_op`, `xor_op`)
- **Rôle** : portes logiques simples (non conformes à une puce réelle)

### `ElementaryNot` (`not`)
- 2 pins : pin 1 = entrée, pin 2 = sortie
- `compute(2)` : récupère pin 1, applique `not_op`

### `NotGate` / `AndGate` / `OrGate` / `XorGate` / `NandGate` / `NorGate` (`4069`, `4081`, `4071`, `4030`, `4011`, `4001`)
- Puces DIP 14 broches réelles avec 4 portes internes chacune
- Pinout conforme au datasheet (pins 7=GND, 14=VCC ignorés)
- `compute(output_pin)` : identifie quelle porte correspond au pin demandé, remonte les inputs

### `Adder4008` (`4008`)
- Additionneur 4 bits complet (DIP 16)
- Pins A1-A4, B1-B4, Cin → Sum1-Sum4, Cout
- `compute()` récupère tous les bits, fait l'addition en tristate bit par bit

### `Selector4512` (`4512`)
- Multiplexeur 8 vers 1 (DIP 16)
- 8 entrées de données, 3 bits de sélection, 1 enable, 1 sortie
- `compute()` décode les bits de sélection et retourne l'entrée correspondante

### `Ram4801` (`4801`)
- RAM 1024×1 bit (DIP 14)
- Adresse 10 bits, 1 bit de données, WE (write enable), CS (chip select)
- Sur front montant de CS avec WE=1 : écrit en mémoire
- Sur CS=1 avec WE=0 : lit la mémoire
- Stockage interne : `std::array<Tristate, 1024> _memory`

---

## 5. L'erreur `NtsError` (`include/error/nts_error.hpp`)

Exception maison héritant de `std::exception`. Utilisée partout pour les erreurs
métier (pin invalide, composant inconnu, format de fichier incorrect...).

```cpp
throw NtsError("Pin invalide");
// capturé par le shell → affiche l'erreur sans crasher
```

---

## 6. Le parseur (`src/parsing/`)

### `ToToken` (`to_token.cpp`)
- Lit le fichier `.nts` ligne par ligne
- Supprime les commentaires (tout ce qui suit `#`)
- Découpe chaque ligne en tokens (mots séparés par espaces/tabulations)
- Retourne une liste de lignes, chaque ligne étant une liste de tokens

### `Parser` (`parser.cpp`)
- Lit les tokens produits par `ToToken`
- Reconnaît deux sections : `.chipsets:` et `.links:`
- **Section chipsets** : chaque ligne `type nom` → crée un `ChipsetInfo{type, nom}`
- **Section links** : chaque ligne `comp1:pin1 comp2:pin2` → crée un `LinkInfo`
- Stocke tout dans `_chipsets` et `_links`

Exemple de fichier `.nts` :
```
.chipsets:
input a        # composant "a" de type "input"
input b
output s
4081 and1      # puce AND réelle

.links:
a:1 and1:1     # sortie de "a" connectée au pin 1 de "and1"
b:1 and1:2
and1:3 s:1     # sortie de "and1" connectée à "s"
```

---

## 7. Le circuit (`src/circuit/circuit.cpp`)

C'est l'orchestrateur central.

### Construction
```cpp
Circuit(chipsets, links)
  → create_components(chipsets)   // instancie tous les composants via la factory
  → create_links(links)           // connecte les pins entre composants
```

### La factory
```cpp
using Creator = std::function<std::unique_ptr<IComponent>(const std::string &)>;

template<typename T>
static std::unique_ptr<IComponent> make(const std::string &name) {
    return std::make_unique<T>(name);
}

static const std::unordered_map<std::string, Creator> s_factory = {
    {"input",  make<InputComponent>},
    {"4081",   make<AndGate>},
    // ...
};
```

- `make<T>` est une fonction générique : `T` est remplacé par la vraie classe au moment
  de l'appel (`make<AndGate>` génère une fonction qui crée un `AndGate`)
- La map associe chaque type string à sa fonction de création
- `it->second(chip.name)` appelle la fonction avec le nom → retourne le composant

### `create_links()`
Pour chaque lien `{comp1, pin1, comp2, pin2}` :
```cpp
comp1->set_link(pin1, *comp2, pin2);
comp2->set_link(pin2, *comp1, pin1);
```
La connexion est **bidirectionnelle** : chaque composant sait où est branché chacun de ses pins.

### `simulate()`
```cpp
for (auto &[name, comp] : _components)
    comp->simulate();
_tick++;
```
Appelle `simulate()` sur tous les composants dans l'ordre (alphabétique car `std::map`),
puis incrémente le compteur de ticks.

### `display()`
Pour chaque input : appelle `get_value()` et affiche `0`, `1`, ou `U`.
Pour chaque output : appelle `get_value()` qui déclenche `compute()` qui remonte le circuit.

---

## 8. Comment `compute()` propage les signaux

C'est **lazy** (paresseux) : rien n'est calculé tant qu'on ne demande pas la valeur.

Exemple : circuit `a → AND → s`

```
display() demande la valeur de s
  → s.get_value() → s.compute(1)
    → suit le lien vers and1, pin 3
    → and1.compute(3)
      → suit le lien vers a, pin 1 → a.compute(1) → retourne _current (ex: 1)
      → suit le lien vers b, pin 1 → b.compute(1) → retourne _current (ex: 0)
      → and_op(1, 0) → retourne 0
    → retourne 0
  → s affiche 0
```

Chaque `compute()` remonte la chaîne récursivement jusqu'aux sources (inputs/true/false).

---

## 9. Le problème des boucles (flip-flop RS)

Dans un circuit avec feedback (ex: NOR croisées), `compute()` serait infini :

```
A.compute() → B.compute() → A.compute() → ... (stack overflow)
```

Solution : `compute_safe()` dans `IComponent`, avec un suivi **par pin** :

```cpp
Tristate compute_safe(std::size_t pin) {
    if (_computing_pins.count(pin))
        return Tristate::Undefined;  // ce pin est déjà en cours → stop
    _computing_pins.insert(pin);
    Tristate result = compute(pin);
    _computing_pins.erase(pin);
    return result;
}
```

Le suivi par pin est crucial : une puce comme la 4081 contient 4 portes AND indépendantes.
Si on chaîne la sortie de la porte 1 (pin 3) vers l'entrée de la porte 4 (pin 13), un suivi
par composant bloquerait le calcul (pin 3 en cours → tout le composant verrouillé → pin 11
retourne U). Avec un suivi par pin, seul le pin 3 est verrouillé, le pin 11 se calcule
normalement.

Tous les appels inter-composants utilisent `compute_safe()` au lieu de `compute()`.

---

## 10. Le shell (`src/shell/shell.cpp`)

Boucle interactive qui lit les commandes sur `stdin` :

| Commande     | Action                                              |
|--------------|-----------------------------------------------------|
| `simulate`   | `circuit.simulate()` — passe au tick suivant        |
| `display`    | `circuit.display()` — affiche inputs et outputs     |
| `loop`       | simule + affiche en boucle jusqu'à Ctrl+C           |
| `a=1`        | `circuit.set_input("a", True)` — prépare le pending |
| `a=0`        | `circuit.set_input("a", False)`                     |
| `a=U`        | `circuit.set_input("a", Undefined)`                 |
| `exit`       | quitte le programme                                 |

Les erreurs `NtsError` sont capturées et affichées sans quitter.

---

## 11. Le `main` (`src/main.cpp`)

```
main(argv[1])
  → Parser(fichier.nts).parse()       // lit et valide le fichier
  → Circuit(chipsets, links)          // construit le circuit
  → Shell(circuit).run()              // lance la boucle interactive
```

Les arguments supplémentaires (`argv[2]`, `argv[3]`...) peuvent être des
assignments initiaux (`a=1 b=0`) appliqués avant le shell.

---

## 12. Les tests unitaires (`tests/`)

Utilise la bibliothèque **Criterion**. Compilés avec `g++ --coverage` pour la
couverture de code (gcovr).

- `test_input_component.cpp` : 16 tests sur `InputComponent`
- `test_output_component.cpp` : tests sur `OutputComponent` + cycle detection
- `test_true_component.cpp` : tests sur `TrueComponent`
- `test_false_component.cpp` : tests sur `FalseComponent`

```bash
make tests_run   # compile, exécute les tests, affiche la couverture (100%)
```

---

## 13. Schéma des dépendances

```
main.cpp
  ├── Parser ──────────────── lit le .nts → ChipsetInfo[], LinkInfo[]
  │     └── ToToken ───────── tokenise le fichier
  │
  ├── Circuit ─────────────── orchestre tout
  │     ├── factory ────────── string → unique_ptr<IComponent>
  │     ├── IComponent (interface)
  │     │     └── AComponent (base concrète)
  │     │           ├── InputComponent
  │     │           ├── ClockComponent
  │     │           ├── OutputComponent
  │     │           ├── TrueComponent / FalseComponent
  │     │           ├── ElementaryGate / ElementaryNot
  │     │           ├── NotGate / AndGate / OrGate / ...
  │     │           ├── Adder4008
  │     │           ├── Selector4512
  │     │           └── Ram4801
  │     └── NtsError ───────── exceptions
  │
  └── Shell ───────────────── commandes interactives → Circuit
```

---

## 14. Flux d'exécution complet

```
1. Lecture du fichier .nts
2. Tokenisation (suppression commentaires, découpe en mots)
3. Parsing (ChipsetInfo et LinkInfo extraits)
4. Création des composants via la factory (unordered_map<string, Creator>)
5. Connexion des pins (set_link bidirectionnel)
6. Boucle shell :
   a. Lecture commande
   b. Si "a=1" → set_input → _pending armé
   c. Si "simulate" → simulate() sur tous les composants → _pending appliqué / clock toggle
   d. Si "display" → compute() récursif depuis les outputs → remonte jusqu'aux inputs
   e. Si "loop" → répète c+d jusqu'à Ctrl+C
```
