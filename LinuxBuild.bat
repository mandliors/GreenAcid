#!/bin/bash 

sh -c Premake/Linux/premake5 gmake2
read -n1 -r -p "Press any key to continue..."