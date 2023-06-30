# Récupération de clé publique ECDSA

Ce code fournit une implémentation en C++ pour récupérer une clé publique à partir d'une signature en utilisant ECDSA (Elliptic Curve Digital Signature Algorithm). Le code utilise la bibliothèque secp256k1 et inclut les fichiers d'en-tête nécessaires.

## Dépendances

- `stdexcept`
- `string`
- `pybind11/pybind11.h`
- `stdio.h`
- `secp256k1_recovery.h`

Assurez-vous d'avoir installé ces dépendances avant de compiler le code.

## Fonction : recover_public_key

La fonction `recover_public_key` permet de récupérer une clé publique à partir d'une signature en utilisant ECDSA. Voici les détails de la fonction :

```cpp
std::string recover_public_key(std::string signature)
```

**Paramètres**
`signature` : La signature à partir de laquelle la clé publique doit être récupérée.

**Valeur de retour**
La fonction renvoie la clé publique récupérée sous forme de chaîne de caractères.

**Exceptions**
La fonction peut générer une exception `std::invalid_argument` si la longueur de la signature est invalide ou si le format de la signature est invalide. Elle peut également générer une exception `std::runtime_error` si la récupération de la clé publique échoue.

**Utilisation en Python**
```python
import public_key
recovered_key = public_key.recover_public_key(signature)
```

**Gestion de l'absence de '0x' dans la signature**

Dans Ethereum et de nombreux autres réseaux de blockchain, les valeurs hexadécimales sont souvent préfixées par " 0x ". Cependant, dans certains cas, la signature que nous recevons peut ne pas commencer par '0x'. Notre fonction `recover_public_key` doit gérer correctement de tels scénarios. 

```cpp
// Check if the signature starts with '0x' and remove it
if (signature.substr(0, 2) == "0x") {
    signature = signature.substr(2);
}
```
La fonction `recover_public_key` peut être appelée depuis Python en fournissant l'argument `signature`, et elle renverra la clé publique récupérée.

## Lancement du projet
- git clone https://github.com/Dahuiss/projet_blockchain_python.git
- cd projet_blockchain_python/publicKey_component
- make
