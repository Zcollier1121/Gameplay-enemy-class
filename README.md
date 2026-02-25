# Gameplay-enemy-class
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
https://github.com/user-attachments/assets/a09fcb1c-af04-449f-9644-901c04e03af2
