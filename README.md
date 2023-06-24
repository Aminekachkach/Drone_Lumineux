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

Après avoir effectué la sélection des composants et défini l'architecture générale de notre PCB (Printed Circuit Board), nous avons choisi d'utiliser deux contrôleurs différents : le microcontrôleur Nucleo STM32L412KB et le SoC QuickLogic Quick Feather EOS S3. L'objectif était de déterminer le contrôleur qui serait le plus adapté à notre configuration spécifique.
Cependant, lors du processus de développement, une mauvaise manipulation a entraîné un dysfonctionnement du bootloader du SoC, le rendant inutilisable (brické). Cette situation a nécessité d'abandonner l'utilisation du SoC Quick Feather EOS S3 pour notre projet.

[Drone_board_STM32_PCB](https://github.com/Aminekachkach/Drone_Lumineux/tree/main/hardware/Drone_board)

<img src="https://github.com/Aminekachkach/Drone_Lumineux/blob/3178f3a68c79a3d7d2b15b143f99031bccc78226/img/Drone_architecture.PNG" width="400" height="300">




## Test Unitaire
Dans notre projet de développement du drone lumineux, nous utilisons des tests unitaires pour vérifier le bon fonctionnement de chaque composant, module et fonctionnalité du code. Les principaux éléments soumis à ces tests sont le driver ESC, le Nopixel et le MPU6050, ainsi que des fonctionnalités spécifiques telles que la configuration des broches PWM et la lecture des données du gyromètre.

### ESC

[Code Moteur](https://github.com/Aminekachkach/Drone_Lumineux/blob/main/firmware/drone/Core/Src/main_moteur.c)

Le driver ESC est spécifiquement conçu pour contrôler les moteurs brushless à courant continu (BLDC). Il reçoit en entrée un signal de modulation de largeur d'impulsion (PWM) qui permet de commander les phases du moteur. Dans notre application, nous utilisons des kits ESC+Moteurs fournis par miniplanes.

Sur le microcontrôleur Nucleo-STM32L412KB, nous allouons 4 broches qui sont connectées à la même chaîne PWM1 afin d'assurer la synchronisation. Veuillez consulter la configuration des broches dans le fichier .ioc correspondant.

PA8  ->  Broche PWM1-1

PA9  ->  Broche PWM1-2

PA10 ->  Broche PWM1-3

PA11 ->  Broche PWM1-4


Le driver fonctionne en configurant un signal PWM avec une période de 20 ms. Le rapport cyclique est ajusté pour envoyer des signaux de 1 ms pour l'initialisation, 1,5 ms pour une vitesse moyenne et 2 ms pour la vitesse maximale. Cela signifie qu'une variation de 1 ms à 2 ms fera tourner les moteurs de la vitesse minimale à la vitesse maximale.

Dans notre code, nous actions les signaux PWM pour les 4 sorties, nous définissons trois constantes de vitesse : Rapport_cyclique_Min, Rapport_cyclique_securite et Rapport_cyclique_Max. À l'aide d'une boucle for, nous faisons varier la vitesse en utilisant les constantes Rapport_cyclique_Min et Rapport_cyclique_securite pour augmenter progressivement la vitesse des moteurs jusqu'à une vitesse raisonnable.


<img src="https://github.com/Aminekachkach/Drone_Lumineux/blob/bf99aa1d844041ec591e66b1d58cae83e2971dcc/img/ESC.PNG">

### MPU6050
[Code MPU](https://github.com/Aminekachkach/Drone_Lumineux/blob/main/firmware/drone/Core/Src/main_mpu.c)

Le MPU6050 est un composant qui combine un gyroscope à trois axes et un accéléromètre à trois axes. Il est utilisé pour mesurer l'orientation, la rotation et l'accélération. Il utilise une communication I2C pour interagir avec notre microcontroleur STM32, de la meme maniere nous allouons donc les deux broches suivantes :

PA7  ->  Broche SCL

PB5  ->  Broche SDA

Au niveau du code, il existe une subtilité importante qui consiste à définir l'adresse du composant shiftée de 1 (0x68 << 1). Cela est nécessaire pour assurer une communication réussie entre le module et le microcontrôleur.

Avant de commencer la configuration, nous effectuons une vérification en interrogeant le registre "Who_am_i" à l'adresse 0x75. Cette étape nous permet de nous assurer que le module communique correctement avec le microcontrôleur. Si la communication est établie, le registre renverra la valeur 0x68.
<img src="https://github.com/Aminekachkach/Drone_Lumineux/blob/main/img/who%20am%20i.png">

Ensuite, nous procédons à la configuration de l'horloge interne à 8 MHz en utilisant le registre "Power management" situé à l'adresse 0x6B. Nous lui attribuons la valeur 0x00 pour sélectionner notre horloge souhaitée.

<img src="https://github.com/Aminekachkach/Drone_Lumineux/blob/main/img/Clock_select%20(2).png">
<img src="https://github.com/Aminekachkach/Drone_Lumineux/blob/main/img/Clock_select%20(1).png">

Nous appliquons également un filtre passe-bas avec une fréquence de coupure de 1 kHz afin d'éliminer les bruits qui pourraient perturber le fonctionnement du composant. Ce filtre est configuré en utilisant le registre situé à l'adresse 0x1A. Nous lui attribuons la valeur 0x05, ce qui correspond à une bande passante de 10 Hz.

<img src="https://github.com/Aminekachkach/Drone_Lumineux/blob/main/img/Lowpass_config.png">
<img src="https://github.com/Aminekachkach/Drone_Lumineux/blob/main/img/bandwidth.png">

Passons maintenant à la configuration du gyromètre en manipulant le registre "Gyro_Config" du MPU6050, situé à l'adresse 0x1B. Ce registre joue un rôle crucial dans la définition de la plage de mesure du gyromètre, qui est fixée à ± 500 °/s. Cette valeur détermine la projection maximale des mesures sur la plage d'acquisition du convertisseur analogique-numérique (ADC).

<img src="https://github.com/Aminekachkach/Drone_Lumineux/blob/main/img/Gyro_config.png">
<img src="https://github.com/Aminekachkach/Drone_Lumineux/blob/main/img/Screenshot%202023-06-24%20120357.png">

En ce qui concerne l'acquisition de données,  il faut prendre en compte la sensibilité du gyromètre, exprimée en LSB (Least Significant Bit) par degré par seconde (°/s). Pour notre plage de mesure de ± 500 °/s, la sensibilité est de 65.5 LSB/°/s comme indiqué sur la figure suivante :

<img src="https://github.com/Aminekachkach/Drone_Lumineux/blob/main/img/LSB_sensitivity.png">


Pour acquérir les données du gyromètre sur les trois axes X, Y et Z, nous devons effectuer une lecture de 6 octets consécutifs, commençant à l'adresse 0x43 et se terminant à l'adresse 0x48. Chaque axe nécessite 2 octets, donc en lisant les 6 octets, nous obtenons les mesures pour les trois axes.
La raison pour laquelle nous lisons ces 6 octets est que les données du gyromètre sont stockées dans des registres consécutifs, un registre pour chaque axe (X, Y et Z). Chaque registre contient 2 octets, qui représentent la valeur de mesure pour cet axe spécifique.

<img src="https://github.com/Aminekachkach/Drone_Lumineux/blob/main/img/read_gyro.png">

Une fois les données du gyromètre acquises, nous devons les ajuster en les divisant par la sensibilité mentionnée précédemment (65.5). Cette étape de correction permet d'obtenir des mesures précises en degrés par seconde, en prenant en considération la sensibilité spécifique du gyromètre.



### Neopixel

[Code Neopixel](https://github.com/Aminekachkach/Drone_Lumineux/tree/Neopixel)

Les LED WS2812  utilisent un protocole de modulation de largeur d'impulsion (Pulse Width Modulation - PWM) pour transmettre les informations de couleur et de luminosité. Dans ce protocole, la valeur d'un bit est déterminée par la durée d'une impulsion.

Pour les deux types de LED, les niveaux de timing à respecter pour encoder un "0" ou un "1" sont généralement les suivants :

Pour un "0" logique :

La durée de l'impulsion haute (HIGH) est généralement d'environ 0,35 μs.
La durée de l'impulsion basse (LOW) est généralement d'environ 0,8 μs.
Pour un "1" logique :

La durée de l'impulsion haute (HIGH) est généralement d'environ 0,7 μs.
La durée de l'impulsion basse (LOW) est généralement d'environ 0,6 μs.

Pour piloter, le Neopixel, nous avons utiliser un timer et DMA pour faire des déplecements de buffers.
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

