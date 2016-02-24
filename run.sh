#!/bin/bash

echo "Results Execution"

echo " IRIS"
./target performers/iris_control_file.txt performers/iris_data.txt > logs/detailed_iris.txt
echo "MUSHROOM"
./target performers/control_mushroom_data.txt performers/mushroom_data.txt > logs/detailed_mushroom.txt
echo "WINE"
./target performers/control_wine_data.txt performers/wine_data.txt > logs/detailed_wine.txt
echo "CHESS"
./target performers/control_chess_data.txt performers/chess_data.txt > logs/detailed_chess.txt
echo "BREAST CANCER"
./target performers/control_breast_cancer.txt performers/breast_cancer_data.txt > logs/detailed_breast_cancer.txt
echo "LIVER"
./target performers/control_liver_data.txt performers/liver_data.txt > logs/detailed_liver.txt
echo "BALANCE"
./target performers/control_balance.txt performers/balance_data.txt > logs/detailed_balance.txt
echo "HABERMAN"
./target performers/control_haberman_data.txt performers/haberman_data.txt > logs/detailed_haberman.txt
echo "TIC TOE"
./target performers/control_tic_toe_data.txt performers/tic_toe_data.txt > logs/detailed_tic_toe.txt

echo " Done .. check logs in logs/ dir.."




