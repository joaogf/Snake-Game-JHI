###instala dependências, as bibliotecas do SDL

sudo apt-get install libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev libsdl-gfx1.2-dev

mydir=$PWD

echo "[Desktop Entry]" > snake_game_jhi_v1.desktop
echo "Name=Snake Game JHI V1" >> snake_game_jhi_v1.desktop
echo "Comment=Jogo Snake Game utilizando a lib JHI-SDL" >> snake_game_jhi_v1.desktop
echo "Exec=$mydir/./play_snake_game.sh" >> snake_game_jhi_v1.desktop
echo "Icon=$mydir/snake_game/images/snake_img.xpm" >> snake_game_jhi_v1.desktop
echo "Categories=Game;Arcade;" >> snake_game_jhi_v1.desktop
echo "Type=Application" >> snake_game_jhi_v1.desktop


echo "cd $mydir/snake_game/bin" > play_snake_game.sh
echo "./snake_game_jhi_v1_3" >> play_snake_game.sh

chmod +x play_snake_game.sh

make
cd snake_game
make
cd ..

sudo mv snake_game_jhi_v1.desktop /usr/share/applications/snake_game_jhi_v1.desktop


