#!/bin/bash
FILE=`zenity --file-selection --title="Select a File"`
case $? i
         0)
                echo "\"$FILE\" selected.";;
         1)
A                echo "No file selected.";;
        -1)
                echo "An unexpected error has occurred.";;
esac