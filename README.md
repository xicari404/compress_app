# 🗜️ Compression Pro - Disk Optimization App

Une application C++ cross-platform pour analyser, compresser et optimiser votre disque dur.

## Fonctionnalités

### 5 Étapes d'Optimisation

1. **Analyse du disque** - Scanne l'espace disque et détecte les fichiers
2. **Résultats** - Affiche un résumé des fichiers trouvés par type
3. **Sélection** - Choix des fichiers à compresser
4. **Compression** - Lance le processus en arrière-plan avec progression
5. **Résultats finaux** - Affiche les économies réalisées

### Navigation Personnalisable

- Barre de navigation en haut avec 5 étapes
- Indication du statut (complété, actuel, désactivé)
- Navigation fluide entre les étapes

### Compression Intelligente

- Utilise **zstandard (zstd)** pour une compression ultra-rapide
- Détection automatique des doublons
- Identification des fichiers par type (vidéo, photo, document, etc.)
- Compression adaptée au type de fichier

### Cross-Platform

- ✅ Windows (détection des disques C:, D:, etc.)
- ✅ macOS (support pour /home et /)
- ✅ Linux (support pour /home et /)

## Requirements

- Qt 6.x
- zstandard library (zstd)
- CMake 3.16+
- C++17 compatible compiler

## Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Utilisation

1. Lancer l'application
2. Suivre les 5 étapes
3. L'app compressera les fichiers sélectionnés

## Architecture

```
┌─────────────────────────────┐
│   MainWindow (QMainWindow)  │
├─────────────────────────────┤
│  NavigationBar (5 étapes)   │
├─────────────────────────────┤
│  QStackedWidget             │
│  ├─ Step1Analysis           │
│  ├─ Step2Results            │
│  ├─ Step3Selection          │
│  ├─ Step4Compression        │
│  └─ Step5Results            │
├─────────────────────────────┤
│ DiskAnalyzer (analyse)      │
│ CompressionEngine (zstd)    │
│ Database (historique)       │
└─────────────────────────────┘
```

## Composants

- **mainwindow.h/cpp** - Fenêtre principale et orchestration
- **navigationbar.h/cpp** - Barre de navigation personnalisée
- **diskanalyzer.h/cpp** - Analyse du disque
- **compressionengine.h/cpp** - Moteur de compression zstd
- **step1-5** - Les 5 étapes de l'interface
- **database.h/cpp** - Stockage des compressions

## Licence

MIT
