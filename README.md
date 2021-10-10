# Metroid-Fusion-Recreation
*Watch the trailer here*
[![Video trailer](https://img.youtube.com/vi/1s1MvxEdPCY/maxresdefault.jpg)](https://youtu.be/1s1MvxEdPCY)
Recreation of Metroid Fusion made in Visual Studio with SDL2. Made as part of my education at Howest Digital Arts and Entertainment.\
As featured on the DAE website: http://www.digitalartsandentertainment.be/article/402/Programming+2+2019+post+mortem \
This was my first major programming assignment, made during the first year of my education. The code in this project isn't fully indicative of my current skills. Still, I believe it was an important milestone since
I went from knowing nothing about programming to being able to recreate part of Metroid Fusion within 10 months.\
For more information, visit my website: https://bryncouvreur.wixsite.com/portfolio

#Controls
- **A** - Move left
- **D** - Move right
- **S** - Crouch
- **W** - Aim upwards
- **UP/DOWN arrows** - Aim diagonally / navigate menu
- **LEFT/RIGHT arrows** - Navigate menu
- **SPACE** - Jump
- **E** - Shoot
- **R** - Toggle missile launcher (after unlocking missile launcher)
- **H** - Restart (on game over screen)

#Some notable functions:
- [Enemy::DetectAvatar](Enemy.cpp#L153-L176)
- [Avatar::HandleKeyPress](Avatar.cpp#L309-L395)
- [Avatar::Shoot](Avatar.cpp#L464-L515)
- [Enemy behavior examples](Moto.cpp#L215-L278)
