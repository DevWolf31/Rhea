# Supresión por Sonido
La supresión por sonido es un nuevo tipo de supresión (tal cómo la de **Falta de Memoria (Out of Memory)**, la de **Conversión de Clase (Class Cast)** o la de **Sobrecarga de la Pila (Stack Overflow)**) que usa una técnica intercambio de entidades-bloque para crear una entidad-bloque en un estado ilegal que luego puede ser usada para suprimir.

#### Créditos
- Descubrimiento:
    - JKM
    - Metacinabar
- Pulido:
    - Igna778
- Investigación y desarrollo:
    - Lame
    - Savva
    - Void
    - Ruthro

> [!TIP]
> Video de demostración: https://youtu.be/77M8Adnxmsw?si=f-hon2OtKevtEUhx
>
> Canal de Youtube:https://youtube.com/@igna778 <br>
> Servidor de Discord: https://discord.gg/rsuTZer7RT

## Aplicaciones
Este exploit tiene una gran cantidad de aplicaciones prácticas, así como una gran probabilidad de convertirse en un método de crasheo si no se usa con el debido cuidado.

## Diseño

### Construirlo
El diseño consiste de un sensor de sculk calibrado, un observador con un bloque de notas y una puerta junto a una alfombra colocada encima del sensor, justo de esta manera:

> [!IMPORTANT]
> Es importante señalar que la puerta en este diseño se encuentra apagada y debería colocarse exactamente de esta manera.

![image-1](https://github.com/igna-778/MinecraftDocs778/blob/main/Sound%20Suppresion%2FPasted%20image%2020240806104831.png)

![image-2](https://github.com/igna-778/MinecraftDocs778/blob/main/Sound%20Suppresion%2FPasted%20image%2020240806105705.png)

La línea de rieles mostrada se encuentra buddeada y debería estar conectada a algún tipo de supresor. Si te encuentras en la 1.20 deberías usar un supresor de shulker por **Conversión de Clase (Class Cast)**, si no, podrías probar el método de supresión más reciente (Por ahora el supresor de bolas de nieve por **Falta de Memoria (Out of Memory)**).

> [!WARNING]
> La supresión por sonido en la 1.19 no es estable ya que no hay manera de controlar los canales de sonido así que deberías probar un supresor de shulker por **Conversión de Clase (Class Cast)** y luego actualizarlo en la 1.20.

### Usarlo
Para usarlo solo párate frente a la puerta mientras esta se abre y se cierra e intenta romper la parte inferior de esta con una azada de diamante, cuando esta se vuelva a abrir el sensor de sculk calibrado se destruirá instantaneamente, activando así el otro supresor.

Después de eso reemplaza la puerta con un comparador y coloca un atril con un **libro de 15 páginas**.

![image-2](https://github.com/igna-778/MinecraftDocs778/blob/main/Sound%20Suppresion%2FPasted%20image%2020240806105705.png)

![image-3](https://github.com/igna-778/MinecraftDocs778/blob/main/Sound%20Suppresion%2FPasted%20image%2020240806111136.png)

> [!TIP]
> Espera unos cuantos minutos hasta que estés seguro de que el servidor haya procesado la supresión, especialmente si estás usando la de **Falta de Memoria (Out of Memory)**.

> [!CAUTION]
> Si el siguiente paso es relizado de manera incorrecta podrías crear un bucle de crasheos. ¡SÉ CUIDADOSO!

Agarra un cofre y mira al comparador, de esa manera te aseguras de que este siempre esté activando la parte trasera del cofre. Coloca el cofre donde el sensor de sculk calibrado solía estar.

![image-3](https://github.com/igna-778/MinecraftDocs778/blob/main/Sound%20Suppresion%2FPasted%20image%2020240806111756.png)

### Destruirlo

> [!CAUTION]
> Si el siguiente paso es relizado de manera incorrecta podrías crear un bucle de crasheos. ¡SÉ CUIDADOSO!

Si no planeas usarlo más rompe el cofre antes que cualquier otro componente.

## Canales
El supresor por sonido tiene 15 canales, pero solo encuentro útiles los siguientes. Si encuentras algún otro uso y piensas que este documento debería ser actualizado contacta a Igna778 a través de Discord.

> [!WARNING]
> Recuerda que las acciones no relacionadas con el jugador (tales como creepers explotando o el uso de cohetes) podrían crashear el servidor.

##### 1 Movimiento
- **Exploit de Kickeo**
    - Kickea cualquier jugador que se mueva.
##### 4 Vuelo con Elítros o Acciones Únicas de Mobs
- **Exploit de Kickeo por Vuelo**
    - Kickea cualquier jugador que vuele con elítros.
- **Ítems Enlazados**
    - Crea ítems enlazados con un soporte de armaduras si es que este tiene brazos.
##### 5 Desmontar Mobs o Equipar Armadura
- **Ítems Enlazados**
    - Crea ítems enlazados de ítems de armadura.
##### 7 Mobs y Jugadores Siendo Dañados
- **Cancelación de Animación de Muerte**
    - Cancela la animación de muerte.
##### 9 Bloques 'Desactivándose'
- **Supresor Pequeño**
    - Usa bloques de notas o trampillas y crea un supresor normal.
##### 10 Bloques 'Activándose'
- **Supresor Pequeño**
    - Usa bloques de notas o trampillas y crea un supresor normal.
##### 12 Bloques Rompiéndose
- **Bloques Indestructibles**
    - Hace a cualquier bloque cercano (excepto el de lana) indestructible.
##### 13 Bloques Colocándose
- **Duplicación**
    - Duplica cualquier bloque que coloques.,

> [!NOTE]
> Por lo que he visto, los otros canales pueden llegar a ser útiles bajo circunstancias muy específicas, pero mayormente solo terminan kickeando jugadores o crasheando el servidor. Debido a esto no son indicados en este documento.

## Código
> [!NOTE]
> Todos los nombres de clases, métodos, campos y cualquier otra cosa son tomados de los mapeos Yarn del juego.

Este exploit es posible gracias al intento de obtener una propiedad que no existe en `blockstate` por parte del código.
Las clases relevantes a este exploit son: `CalibratedSculkSensorBlockEntity`, `State` y `Vibrations`.

#### Jerarquía de Llamadas
Cuando se crea un evento (en este caso una vibración) el juego busca escuchas a los que darle esa información. En este caso `CalibratedSculkSensorBlockEntity` es uno de esos escuchas y el código que determina si el evento debe ser aceptado es este:

```java
public boolean listen(ServerWorld world, RegistryEntry<GameEvent> event, GameEvent.Emitter emitter, Vec3d emitterPos) {  
    ListenerData listenerData = this.receiver.getVibrationListenerData();  
    Callback callback = this.receiver.getVibrationCallback();  
    if (listenerData.getVibration() != null) {  
        return false;  
    }  
    if (!callback.canAccept(event, emitter)) {  
        return false;  
    }  
    Optional<Vec3d> optional = callback.getPositionSource().getPos(world);  
    if (optional.isEmpty()) {  
        return false;  
    }  
    Vec3d vec3d = optional.get();  
    if (!callback.accepts(world, BlockPos.ofFloored(emitterPos), event, emitter)) {  
        return false;  
    }  
    if (VibrationListener.isOccluded(world, emitterPos, vec3d)) {  
        return false;  
    }  
    this.listen(world, listenerData, event, emitter, emitterPos, vec3d);  
    return true;
}
```

En la primera llamada a `accepts` es donde ocurre uno de los crasheos.

```java
public boolean accepts(ServerWorld world, BlockPos pos, RegistryEntry<GameEvent> event, @Nullable GameEvent.Emitter emitter) {  
    int i = this.getCalibrationFrequency(world, this.pos, CalibratedSculkSensorBlockEntity.this.getCachedState());  
    if (i != 0 && Vibrations.getFrequency(event) != i) {  
        return false;  
    }  
    return super.accepts(world, pos, event, emitter);  
}
```

Si `blockstate` no tiene una propiedad de dirección este crasheará en `getCalibrationFrequency` antes de hacer la comprobación por el canal de la vibraciòn, si no este verá si la vibración y la frecuencia establecida por la potencia de la redstone corresponden entre sí y si es así, este crasheará en `super.accepts`.

## Créditos del Documento
Gracias a todos los que contribuyeron en este documento.
#### Creador
- Igna778
#### Contribuyentes
- krispyking24
- infernal
- rca
- DevWolf31