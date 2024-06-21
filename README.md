# Flight_Booking_System

This program allows users to books flight tickets and also check for available flights. It also has an admin access to manage and update flight data.   
It is advised that users read the instructions given below before using the program or in case of any query.

# Note
- Data regarding flights and airports will be added soon.
- Input validity checks as of now aren't performed at all portions, will be fixed soon.

# Concepts

- Object Oriented Programming
- File Handling
- Multithreading
- Data Structures

# How to run the program

1. Copy all the files into a folder.
2. Open your Windows terminal (or VS Code terminal). Note that some Windows specific functions have been used so a Windows system is required for the functioning of the program.
3. Set your terminal window to full screen (or as big as possible).
4. Use the command "g++ main.cpp -pthread" (don't use quotes).
5. Run the .exe file.
6. Your program has now been initiated!

# Overview of various sections of the program :

## 1. Guest

- No sign up required.
- Can check for available flights.
- Cannot book ticket.

## 2. User

- Sign up is required. (For now, login page is not created so sign up is not required)
- Can check for available flights
- Can book ticket.

## 3. Admin

- Has entire access over the data.
- Can add or delete data.
- Can check existing data.

## 4. Booking a Ticket

- Once you enter your flight and passenger details, a .html file will be generated (in the same folder) containing your ticket.
- Simply double click on the .html file to open your ticket. 

# Managing data as an Admin

The two main components are airplane(flight) and airport data. Before adding any airplane, there must be an airport at the starting and destination locations. Hence, airports must added first. To add an airplane, first its respective airline, airplane model and route must be added.The, you can add a flight(airplane). To check what exact details are to be added, for each entity, you can check out the already existing data through the admin section (for each entity, you will find an option as "View ... List").

# Instructions

- After each stage of entering data, you will be asked to choose Yes/NO/Quit. "Yes" means your input is correct and you want to proceed further. "No" means your input is incorrect and you want to entire your input again. "Quit" means you want to quit from that section (stop giving input) and go to homepage.
- To go back, use the ESCAPE button on your keyboard. Only instances where ESCAPE won't work is when your in a homepage(guest/user/admin). In that case, you will find the log out option, choose that.
- Although rapidly giving inputs to the system should be fine in most cases but it is always preferred to give one input after another and give the program sufficient time to load its screen.
- In case you find that you are pressing your keyboard keys but the program apparently isn't taking your input, it's probably because the cursor has moved to a place outside your terminal screen (probably due to changing of tabs). As a remedy, try left clicking once on the terminal screen and then continue giving your input.
- Measures have been taken to tackle incorrect inputs but it is advisable to give correct inputs.
<!-- - Regarding ticket css file. -->

# References

- ASCII Art for title taken from patorjk.com
- ASCII Art for flight animation taken from asciiart.eu
- Flight image on ticket taken from vecteezy.com
- Information regarding ANSI Escape Sequences : https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797

# Disclaimer

This program does not book tickets in the real life. All the names of various entities used in this program are just for the sake of this program and do not correlate with the entities in real life.
