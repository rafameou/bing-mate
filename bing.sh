#!/bin/sh
echo "Downloading!"
JSON=$(curl -s "https://www.bing.com/HPImageArchive.aspx?format=js&idx=0&n=1&mkt=pt-BR")
wallpaper_path=$(echo $JSON | jq -r ".images[0].url")
title=$(echo $JSON | jq -r ".images[0].title")
startdate=$(echo $JSON | jq -r ".images[0].startdate")

if [ -f "w/$startdate.jpg" ]; then
    echo "Today's wallpaper was already downloaded."
else 
	curl -o w/$startdate.jpg "www.bing.com$wallpaper_path"
fi

echo "Setting \"$title\" as wallpaper!"
gsettings set org.mate.background picture-filename $(pwd)/w/$startdate.jpg

echo "Good-Bye!"



