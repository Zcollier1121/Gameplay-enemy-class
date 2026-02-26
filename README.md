 Gameplay-enemy-class
This C++ class contains the data and functionality for the ghost enemy in Specter Inspector.
All logic is managed within the class keeping it uncoupled and modular.

# Functionality
- Movement: Patrol and Chase
- Stun
- Invisibility
- serialization
  
# Movement
A ghost can have one of two movement types Patrol and Chase.
Patrol follows a linear path from point A to point B through lerping.

[(README Assets/Ghost Chase.png)](https://github.com/user-attachments/assets/fcec0873-53ce-466f-9839-deb3d58e4590)

https://github.com/user-attachments/assets/9f496894-b658-4d68-9b40-ca96d9682194

# Stun
Ghosts can be stunned and frozen in place for a set amount of time. Becoming intangible and unable to hurt the player.

https://github.com/user-attachments/assets/d7c3257a-b29f-4fce-8ba4-07808f4e31bc

# Invisibility
Ghosts slowly become visible the closer they are to the player. Starting completely invisible when they are far away, gaining a particle effect at medium range, and becoming fully visible when near the player.

https://github.com/user-attachments/assets/8c4191c0-848c-42e6-8022-81e9e8ef03a9

#Serialization
Ghosts can be serialized and deserialized into a json file to allow for designers to quickly create and iterate. Ghosts can have their spawn, movement type, patrol path, speed, etc modified.
