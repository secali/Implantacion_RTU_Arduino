
<a href="https://github.com/secali/"><img src="Implantacion_RTU_Arduino/blob/f8837cb275974dfaa43483fa9f56f2a73ff4929a/circuito.JPG"></img></a>

El problema a resolver es la falta de seguridad frente a acumulación de gases o escapes de 
gas en una nave industrial, donde se trabaja con gases CO2 constantemente y se puede llegar a 
acumular a niveles tóxicos para la salud. Usaremos los siguientes elementos, que vienen 
descritos acontinuación.

Un sensor de gas que detecte por niveles cuando hay acumulación de gas, un servo 
motor que abrirá una escotilla 45º cuando el nivel de gas sea bajo, y a 90º cuando el nivel de 
gas sea medio.

En paralelo cuando el nivel de gas sea medio, un motor de CC (que simula un ventilador)
entrará en funcionamiento cuando el nivel de gas sea alto. El control de este se hace con un 
transistor que BJT(NPN) que simula un relé que hace de controlador de potencia, ya que en un 
entorno industrial habrá un ventilador industrial que dependiendo de cómo sea de grande, su 
motor, trabajará con altos voltajes e intensidades.

Además, como medida extra cuando el sensor de proximidad detecte a alguien en la estancia 
se emitirá una señal sonora a través del buzzer que simula un altavoz a distintas frecuencias e 
intensidades dependiendo del nivel de gas. Y un led RGB que simularía luces de emergencia, 
que dependiendo del nivel de gas se encenderían en distintos colores dependiendo del nivel de 
gas: verde (no hay gas o nivel muy bajo y no peligroso), amarillo (nivel medio), rojo (nivel 
alto).
