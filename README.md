# Flight Booking System

This program allows users to book flight tickets and also check for available flights. It also has admin access to manage and update flight data.   
It is advised that users read the instructions given below before using the program or in case of any query.

# Disclaimer

This program does not book tickets in real life. All the names of various entities used in this program are just for the sake of this program and do not correlate with the entities in real life.

# Note
- Flight data has been added for flights connected with Bhubaneswar Airport. More data will be added soon.
- Input validity checks aren't being performed at some of the sections, will be fixed soon.
- Login screen is yet to be added.

# Concepts

- Object Oriented Programming
- File Handling
- Multithreading
- Data Structures

# How to run the program

1. Copy all the files into a folder or clone the repository.
2. Open your Windows terminal (or VS Code terminal). Note that some Windows-specific functions have been used so a Windows system is required for the functioning of the program.
3. Set your terminal window to full screen (or as big as possible).
4. Use the following command to compile the code :
   ```bash
   g++ main.cpp -o main.exe -pthread
   ```

5. In case of an error related to "pthread.h", if you are using MinGW, please make sure that the "mingw32-pthreads-w32" package has been installed on your system. If not, then open your MinGW Installer and install the package.

6. Run the executable file using the following command :
   ```bash
   ./main.exe
   ```

7. Your program must now be running.

# Overview of various sections of the program

## 1. Guest

- No sign up required.
- Can check for available flights.
- Cannot book ticket.

## 2. User

- Sign-up is required. (For now, login page is not created so sign-up is not required)
- Can check for available flights.
- Can book a ticket.

## 3. Admin

- Has entire access to the data.
- Can add or delete data.
- Can check existing data.

## 4. Booking a Ticket

- Once you enter your flight and passenger details, a Flight_Ticket.html file will be generated (in the same folder), which will contain your ticket.
- Simply double-click on the Flight_Ticket.html file to open your ticket. 

# Managing data as an Admin

The two main components are airplane(flight) and airport data. Before adding any airplane, there must be an airport at the starting and destination locations. Hence, airports must added first. To add an airplane, first, its respective airline, airplane model and route must be added. Then, you can add a flight(airplane). To check what exact details are to be added, for each entity, you can check out the already existing data through the admin section (for each entity, you will find an option as "View [entity name] List").

# Instructions

- All times are displayed in 24-hour format.
- If you want to select an option with the serial number, let's say '2', then press '2' on your keyboard to select that option.
- After each stage of entering data, you will be asked to choose Yes/NO/Quit. "Yes" means your input is correct and you want to proceed further. "No" means your input is incorrect and you want to entire your input again. "Quit" means you want to quit from that section (stop giving input) and go to the homepage.
- To go back to a previous page, use the ESCAPE button on your keyboard. The only instances where ESCAPE won't work is when you are on a homepage(guest/user/admin). In that case, you will find the log out option, choose that.
- Although rapidly giving inputs to the system should be fine in most cases, it is always preferred to give one input after another and give the program sufficient time to load from one section to another.
- In case you find that you are pressing your keyboard keys but the program isn't taking your input, it's probably because the cursor has moved to a place outside your terminal screen (probably due to changing tabs). As a remedy, try left-clicking once on the terminal screen and then continue giving your input.
- Measures have been taken to tackle invalid inputs but it is advisable to give valid inputs.
- If you move the ticket file to some other folder, please also move style.css and airplane_image.png files along with it.
- If you feel that the program isn't responding as you intended, then kill the terminal and restart your program.

# Resources

- ASCII Art for title : [patorjk.com](https://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20)
- ASCII Art for flight animation : [asciiart.eu](https://www.asciiart.eu/vehicles/airplanes)
- Flight image on ticket : [vecteezy.com](https://www.vecteezy.com/png/37277848-flying-airplane-isolated-on-background-3d-rendering-illustration)
- Information regarding ANSI Escape Sequences : [gist.github.com/fnky](https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797)
