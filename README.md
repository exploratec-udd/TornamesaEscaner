# Manual de usuario para tornamesa de escáner 3D

El Tornamesa de escáner 3D de ExploraTec es una herramienta de licencia abierta de apoyo a la hora de utilizar cualquier tipo de escáner 3d, facilitando el proceso de escanear objetos al rotarlos por el usuario. Se controla a través de una página web alojada en una red local de WiFi

![Tornamesa_Render](https://github.com/user-attachments/assets/c2f2c9d9-a9e0-44d0-9200-01591babeb82)

## Componentes:

### Electrónica:

- #### ESP-32 Dev kit

  El cerebro de la herramienta, utilizado para controlar señales al motor, alojar la pagina web y crear la red local.

- #### Motor Stepper Bipolar Kysan 1124090 Nema 17

  La fuerza del dispositivo, al ser un Motor Stepper nos permite movimientos controlados y precisos.

- #### Módulo Shield para driver A4988 y driver

  Quien facilita el control de el Motor Stepper, permitirndo además la alimentación del mismo.

- #### Fuente de poder de 9V
  Necesaria pala la alimentación del motor Stepper a través del Modulo Shield

- #### Regulador de voltaje lm2596 dc-dc hw 411 Step Down 

   Quien permite tener una fuente de poder para el dispositivo, al poder usar la fuente de mayor voltaje del motor para poder alimentar a la ESP 32.

 - #### Sensor Hall magnético KY-003

    Para determinar donde se encuentra la rotación del motor y calibración

 |<!-- -->|<!-- -->|
 |:---:|:---:|
 |ESP-32| Motor Stepper|
 |![ESP32](https://github.com/exploratec-udd/TornamesaEscaner/blob/main/imagenes2/ESP32-DevKit.jpg)|![Kysan 1124090](https://github.com/exploratec-udd/TornamesaEscaner/blob/main/imagenes2/kysan_1124090.jpg)|
 |Módulo Shield|A4988|
 |![Módulo Shield](https://github.com/exploratec-udd/TornamesaEscaner/blob/main/imagenes2/Shield.jpg)|![A4988](https://github.com/exploratec-udd/TornamesaEscaner/blob/main/imagenes2/A4988.jpg)|
 |Fuente de poder|Regulador de voltaje LM2596|
 |![Fuente de poder 9V](https://github.com/exploratec-udd/TornamesaEscaner/blob/main/imagenes2/Transformador_de_Corriente.jpg)|![lm2596 dc-dc hw 411 Step Down](https://github.com/exploratec-udd/TornamesaEscaner/blob/main/imagenes2/LM2596_Regulador_Step_Down.jpg)|
 |Sensor Hall magnético KY-003|
 |![Fuente de poder 9V](https://github.com/exploratec-udd/TornamesaEscaner/blob/main/imagenes2/Sensor_Hall.jpg)|
### Hardware:

- #### Componentes Impresos del cuerpo de la mesa

  Las piezas que conforman la estructura de este dispositivo

- #### Pernos e insertos M3

  Encargados de sostener los componentes impresos del Tornamesa

- #### Imán de Neodimio circular 10x3mm

  Necesario para la calibración del dispositivo, es censado por el sensor Hall.  

- #### Rodamiento NSK 6205 C3 ( 25mm x 52mm x 15mm )
  
  Usadas para alivianar el roce y sostener la base de giro

- #### Canicas de 10mm
  
  Usadas para alivianar el roce y equilibrar la superficie de la mesa

- #### Superficie circular (Mínimo 12 Cm)

  Superficie de la mesa donde se colocan los objetos que se deseen escanear, material a elección del usuario.

  
### Software:

- #### Arduino IDE

  Un entorno de desarrollo integrado que nos permite programar y actualizar el cerebro del sistema.

 - #### Navegador Web

  El dispositivo al ser controlado por una pagina web local es necesario un navegador para utilizarlo.

 |Arduino IDE|Navegador Web|
 |:---:|:---:|
|![image](https://github.com/exploratec-udd/TornamesaEscaner/blob/main/imagenes2/Arduino_IDE.png)|![image](https://github.com/exploratec-udd/TornamesaEscaner/blob/main/imagenes2/Navegadores.jpg)|

## Vista explosionada:
![image]()



## Instrucciones de uso:

### Encendido

Al encender el dispositivo este como primer paso iniciará su calibración, girará en sentido anti-horario para encontrar su punto de inicio:

![GifInicio](https://github.com/exploratec-udd/TornamesaEscaner/blob/main/imagenes2/Calibracion.gif)

### Conección y pagina web

Por defecto, el nombre de la red que debes conectarte para controlar el tornamesa es "ExploraTec Plataforma de Escaner" la cual es una red que permite **hasta 1 usuario por sesion**. La contraseña por defecto es **"12345678"** para acceder a esta y si es accedida a través del telefono celular es posible que pregunte si deseas mantenerte en la red debido a que, al ser una red local, no posee conección al internet.
|<!-- -->|<!-- -->|
|:---:|:---:|
|![image]()|![image]()|
|![image]()|![image]()|

Al estar dentro de la red local debes entrar a tu navegador y entrar a la pagina web de control. Por defecto el link de esta es el 192.168.4.1. Si no puedes entrar a esta, es posible que tu dispositivo te haya conectado a otra red en busca de una red coon internet.

|<!-- -->|<!-- -->|
|:---:|:---:|
|![image]()|![image]()|

### Control del dispositivo
