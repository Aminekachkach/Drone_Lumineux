[![Contributors][contributors-shield]][contributors-url]
[![LinkedIn][linkedin-shield]][linkedin-url]
[![LinkedIn2][linkedin-shield2]][linkedin-url2]


<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/Aminekachkach/Drone_Lumineux">
    <img src="img/logo.jpg" alt="Logo" width="300" height="300">
  </a>

<h3 align="center">Drone Lumineux</h3>

  <p align="center">
    A magic drone
</div>


<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table des matières</summary>
  <ol>
    <li>
      <a href="#Introduction">Introduction</a>
      <ul>
        <li><a href="#organigramme-des-tâches-du-projet">Organigramme des tâches du projet</a </li>
        <li><a href="#architecture-générale">Architecture générale</a </li>
      </ul>
    </li>
    <li>
      <a href="#built-with">Build with</a>
    </li>
    <li>
      <a href="#test-initaire">Test Unitaire</a>
    <ul>
        </li><a href="#ESC">ESC</a></li>
        <li><a href="#MPU6050">MPU6050</a></li>
        <li><a href="#Neopixel">Neopixel</a></li>
    </ul>

  </ol>
</details>

## Introduction  
Le projet Drone Lumineux vise à concevoir un drone équipé d'un jeu de lumières. L'objectif est de créer une expérience visuelle unique en combinant les fonctionnalités d'un drone avec des effets lumineux spectaculaires.

Pour cela,  nous avons établi notre projet en utilisant une approche agile avec Scrum et les concepts d'épique (epic), de fonctionnalité (feature) et d'histoire utilisateur (user story). 

## Built With

* [![C][C.js]][C-url]
* [![Cube][Cube.js]][Cube-url]


### Organigramme des tâches du projet

<img src="https://github.com/Aminekachkach/Drone_Lumineux/blob/657cf031782364ef576185f705b6a646cfb6faea/img/Epic.PNG" alt="EPIC" width="450" height="250">

<img src="https://github.com/Aminekachkach/Drone_Lumineux/blob/5e0647ed3254db6fd831a457342b895aebda767c/img/Features.PNG" alt="FEATURES" width="450" height="250">

<img src="https://github.com/Aminekachkach/Drone_Lumineux/blob/5e0647ed3254db6fd831a457342b895aebda767c/img/Prioritization.PNG" alt="User_story" width="450" height="250">


En utilisant le modèle, nous avons identifié et défini les tâches qui correspondent aux histoires utilisateurs. L'objectif est de fournir rapidement de la valeur à notre produit en travaillant sur ces tâches de manière itérative. 
 
### Architecture générale

Nous avons effectué la sélection des composants et défini l'architecture générale pour concevoir le PCB (Printed Circuit Board).  
[Drone_board_PCB](https://github.com/Aminekachkach/Drone_Lumineux/tree/main/hardware/Drone_board)

<img src="https://github.com/Aminekachkach/Drone_Lumineux/blob/3178f3a68c79a3d7d2b15b143f99031bccc78226/img/Drone_architecture.PNG" width="400" height="300">




## Test Unitaire
Pour chaque test unitaire, nous avons une branche dédiée. 

### ESC

<img src="https://github.com/Aminekachkach/Drone_Lumineux/blob/bf99aa1d844041ec591e66b1d58cae83e2971dcc/img/ESC.PNG">

### MPU6050


### Neopixel

[Neopixel_branche](https://github.com/Aminekachkach/Drone_Lumineux/tree/Neopixel)

Les LED WS2812  utilisent un protocole de modulation de largeur d'impulsion (Pulse Width Modulation - PWM) pour transmettre les informations de couleur et de luminosité. Dans ce protocole, la valeur d'un bit est déterminée par la durée d'une impulsion.

Pour les deux types de LED, les niveaux de timing à respecter pour encoder un "0" ou un "1" sont généralement les suivants :

Pour un "0" logique :

La durée de l'impulsion haute (HIGH) est généralement d'environ 0,35 μs.
La durée de l'impulsion basse (LOW) est généralement d'environ 0,8 μs.
Pour un "1" logique :

La durée de l'impulsion haute (HIGH) est généralement d'environ 0,7 μs.
La durée de l'impulsion basse (LOW) est généralement d'environ 0,6 μs.

Pour piloter, le Neopixel, nous avons utiliser un timer et DMA pour faire des déplecement de buffers.
Le code source provient d'un driver existant.



<img src="https://github.com/Aminekachkach/Drone_Lumineux/blob/b4e780a0b6354f6479916d1c8b3c014ff12debe8/img/Neopixel.PNG" width="400" height="200">


<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]:https://img.shields.io/badge/CONTRIBUTORS-2-green
[contributors-url]: https://github.com/Aminekachkach/Drone_Lumineux/graphs/contributors
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/bilal-marecar/
[linkedin-shield2]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url2]: https://www.linkedin.com/in/amine-kachkach/
[product-screenshot]: images/screenshot.png
[C.js]: https://img.shields.io/badge/LanguageC-darkblue
[C-url]: https://www.gnu.org/software/gnu-c-manual/gnu-c-manual.html
[Cube.js]: https://img.shields.io/badge/STM32Cube-lightblue
[Cube-url]: https://www.st.com/en/ecosystems/stm32cube.html/

