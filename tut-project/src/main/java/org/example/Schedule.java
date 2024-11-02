package org.example;

import java.io.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Scanner;
import static java.lang.Character.isLetter;

public class Schedule {

    public static String[][] timetable= new String[10][6];

    public static  int venueList = 1;
    public static  int moduleList = 2;
    public static  Module module = null;
    public static  Venue venue = null;

    public Schedule(){

    }
    public void printarr(String[] arr){
        for (int i = 0; i < arr.length; i++){
            System.out.print(arr[i] + " ");
        }
        System.out.println();
    }

    public void printMdlLst(ArrayList<Module> modulelst){
        for (Module mdl:modulelst) {
            System.out.println(mdl.getModuleName() + ", " + mdl.getNumStudents()+", "+mdl.getLectureName());
        }
    }

    public void printVneLst(ArrayList<Venue> venuelst){
        for (Venue vn: venuelst) {
            System.out.println(vn.getRoomNum()+", " + vn.getVenueCapacity());
        }
    }
    public boolean isValid(String line, int inputType){
        String[] data =  line.split(",");
        int dataLen = data.length;
        if(inputType == venueList && dataLen != 2){
            return  false;
        }else if(inputType == moduleList && dataLen != 5){
            return  false;
        } else {
            return isRightOrder(data,inputType);
        }
    }

    private boolean isRightOrder(String[] data, int inputType) {
        //removing white spaces
        for (int i = 0; i < data.length; i++) {
            String name = data[i].strip();
            data[i] = name;
        }

        if(inputType == venueList){
            try {
                int roomsize = Integer.parseInt(data[1]);
            }catch (Exception e){
                return  false;
            }
            //adding venues
            if (venue != null) {
                for (int i = 0; i < data.length; i++) {
                    if(venue.getSizeVenueList() > 0){
                        if(!venue.isVenueFound(data[i])){
                            venue.addVenueName(data[i]);
                        }
                    }else {
                        venue.addVenueName(data[i]);
                    }
                }
            }
        } else if (inputType == moduleList) {
            if(validDay(data[3])==false){
                return false;
            }
            try {
                boolean isFirstCharLecturer = isLetter(data[0].charAt(0));
                if(isFirstCharLecturer && isvalidName(data[2])){
                    int roomNumber = Integer.parseInt(data[1]);
                }else {
                    return false;
                }
            }catch (Exception e){
                return  false;
            }
            //adding module names
            if (module != null) {
                for (String datum : data) {
                    if (module.getSizeModuleList() > 0) {
                        if (!module.isModuleFound(datum)) {
                            module.addModuleName(datum);
                        }
                    } else {
                        module.addModuleName(datum);
                    }
                }
            }
        }
        return true;
    }

    public boolean validDay(String datum) {
        String day = datum.toLowerCase();
        if(day.equals("monday")){
            return true;
        } else if (day.equals("tuesday")) {
            return true;
        } else if (day.equals("wednesday")) {
            return true;
        } else if (day.equals("thursday")) {
            return true;
        } else if (day.equals("friday")) {
            return true;
        } else if (day.equals("saturday")) {
            return true;
        } else if (day.equals("sunday")) {
            return true;
        }
        return false;
    }

    public boolean isvalidName(String datum) {
        int hyphenCount = 0;
        int spaceCount = 0;
        for (int i = 0; i < datum.length(); i++) {
            if(isLetter(datum.charAt(i)) || datum.charAt(i) == '-' || datum.charAt(i) == ' '){
                if (datum.charAt(i) == '-' && hyphenCount > 1){
                    return false;
                }else if(datum.charAt(i) == '-'){
                    hyphenCount+=1;
                }
                if (datum.charAt(i) == ' ' && spaceCount > 4){
                    return false;
                }else if(datum.charAt(i) == ' '){
                    spaceCount+=1;
                }
            }else {
                return false;
            }
        }
        return true;
    }

    public void makeTimeTable(){
        timetable[0][0] = "DAY/TIME";
        for (int i = 1; i < 10; i++) {
            int time = 7+i;
            String item = time+":00";
            if(time < 10){
                item = "0"+item.concat(" AM");
            }else if(time > 9 && time < 12){
                item = item.concat(" AM");
            }else {
                item = item.concat(" PM");
            }
            timetable[i][0] = item;
        }

        timetable[0][1] = "Monday";
        timetable[0][2] = "Tuesday";
        timetable[0][3] = "Wednesday";
        timetable[0][4] = "Thursday";
        timetable[0][5] = "Friday";
    }
    public void updateTimetable(String venue,String time, String day,String module,String lecture){
        String[] sec = time.split(":");
        int hr2 = Integer.parseInt(sec[0].strip());
        for(int i = 1; i < timetable.length; i++){
            String[] first = timetable[i][0].split(":");
            int hr = Integer.parseInt(first[0].strip());
            if (hr == hr2){
                int col = getColumn(day);
                if( col > 0){
                    timetable[i][col] = "venue:"+venue+":"+"module:"+module+":Lecturer:"+lecture;
                }
            }
        }
    }

    public void printTimetable(String filename){
        String outputFilepath = "./Output/"+filename+".txt";
        try{
            FileWriter fileWriter = new FileWriter(outputFilepath,true);
            BufferedWriter bufferedWriter = new BufferedWriter(fileWriter);
            for (int i = 1; i < timetable.length; i++) {
                for (int j = 1; j < 6; j++) {
                    if(timetable[i][j] == null){
                        continue;
                    }
                    String day = timetable[0][j]+"\n";
                    String time = timetable[i][0]+"\n";
                    String[] data =  timetable[i][j].split(":");
                    String venue = data[0]+": ";
                    String venuename = data[1]+"\n";
                    String module = data[2]+": ";
                    String moduleName = data[3]+"\n";
                    String lecture = data[4]+": ";
                    String lectureNmae = data[5]+"\n";

                    bufferedWriter.write(day);
                    bufferedWriter.write(time);
                    bufferedWriter.write(venue);
                    bufferedWriter.write(venuename);
                    bufferedWriter.write(module);
                    bufferedWriter.write(moduleName);
                    bufferedWriter.write(lecture);
                    bufferedWriter.write(lectureNmae);
                    bufferedWriter.write("\n");
                    
                }

            }
            bufferedWriter.close();
        }catch (IOException e){
            for (int i = 1; i < timetable.length; i++) {
                for (int j = 1; j < 6; j++) {
                    if(timetable[i][j] != null){
                        String day = timetable[0][j];
                        String[] data =  timetable[i][j].split(":");

                        
                        String venue = data[0]+": ";
                        String venuename = data[1];
                        String module = data[2]+": ";
                        String moduleName = data[3];
                        String lecture = data[4]+": ";
                        String lectureNmae = data[5];

                        System.out.println(day);
                        System.out.println(venue+venuename);
                        System.out.println(module+moduleName);
                        System.out.println(lecture+lectureNmae);

                    }
                }
                System.out.println();
            }
        }

    }

    public int getColumn(String day){
        String dayUpper = day.toUpperCase();
        for (int i = 1; i < 6; i++) {
            String tblUpper = timetable[0][i].toUpperCase();
            if(tblUpper.equals(dayUpper)){
                return i;
            }
        }
        return -1;
    }

    public String[][] getTimeTable(){
        return timetable;
    }

    public  ArrayList<? extends Object> myreadFile(String filepath, File file,int inputType) {
        String[] arrInput = null;
        Scanner myReader = null;

        ArrayList<Venue> venues = new ArrayList<>();
        ArrayList<Module> modules = new ArrayList<>();
        int i = 0;
        try {
            myReader = new Scanner(file);
            while (myReader.hasNextLine()) {
                String data = myReader.nextLine();
                if(isValid(data,inputType)){
                    arrInput =   data.split(",");
                    String in1 =  arrInput[0].strip();
                    String in2str =  arrInput[1].strip();
                    int input2 = Integer.parseInt(in2str);
                    if(inputType == venueList){
                        Venue venue1 = new Venue(in1,input2,false,"");
                        venues.add(venue1);
                    }else {
                        String input3 = arrInput[2].strip();
                        String input4 = arrInput[3].strip();
                        String input5 = arrInput[4].strip();
                        Module module1 = new Module(in1,input2,input3,input4,input5,"");
                        modules.add(module1);
                    }
                }else{
                    String outputFilePath = "outputErr.txt";
                    try {
                        FileWriter fileWriter = new FileWriter(outputFilePath, true);
                        BufferedWriter bufferedWriter = new BufferedWriter(fileWriter);

                        String thing = "";
                        String expected= "";
                        String example = "";
                        if(inputType == venueList){
                            thing = "VENUE";
                            expected = "Room name/number,room capacity";
                            example = "Room21,50";
                        }else {
                            thing = "MODULE";
                            expected = "Module name,Number of students registerd for the module,Lecturer,time";
                            example = "Mathematics,18,john,Monday 10:00 AM - 12:00 PM";
                        }
                        bufferedWriter.write("Wrong "+thing+" input\n");
                        bufferedWriter.write("Expected "+expected+"\n");
                        bufferedWriter.write("Example "+example+"\n");
                        bufferedWriter.close();

                    } catch (IOException e) {
                        String thing = "";
                        String expected= "";
                        String example = "";
                        if(inputType == venueList){
                            thing = "VENUE";
                            expected = "Room name/number,room capacity";
                            example = "Room21,50";

                        }else {
                            thing = "MODULE";
                            expected = "Module name,Number of students registerd for the module,Lecturer,time";
                            example = "Mathematics,18,john,Monday 10:00 AM - 12:00 PM";
                        }
                        System.out.println("Wrong "+thing+" input" );
                        System.out.println("Expected "+expected);
                        System.out.println("Example "+example);
                    }

                    System.exit(0);
                }
                i+=1;
            }
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        }
        if(inputType == venueList){
            return  venues;
        }
        return modules;
    }

    public void scheduling(ArrayList<Module> modulelst, ArrayList<Venue> venuelst) {

        int i = 0;
        int modindx = 0;
        int venindx = 0;
        int assignmod = 0;
        while (venindx < venuelst.size() && assignmod < modulelst.size()) {
            for (Module m : modulelst) {
                if (compatibility(modulelst.get(modindx), venuelst.get(venindx))) {
                    Module mod = modulelst.get(modindx);
                    Venue ven = venuelst.get(venindx);
                    m.setRoomNum(venuelst.get(i).getRoomNum());
                    venuelst.get(i).setOccupied(true);
                    updateTimetable(ven.getRoomNum(),mod.getTheHour(mod.getLectureTime()),
                            mod.getDayOfWeek(),mod.getModuleName(),mod.lectureName);
                    assignmod += 1;
                    i += 1;
                    modindx += 1;
                    venindx += 1;

                }else {
                    venindx += 1;
                }

            }
        }
    }

    public boolean compatibility(Module module1, Venue venue1) {

        if(venue1.getIsOccupied()){
            return false;
        } else if (venue1.getVenueCapacity() < module1.getNumStudents()) {
            return false;
        }
        return true;
    }

    public static void main(String[] args) {
        //TODO first input will be the list of venues
        Schedule schedule = new Schedule();
        String filepathModule = "./src/main/resources/Modules1";
        String filepathVenue = "./src/main/resources/Venues1";
        ArrayList<Module>  modulelst = new ArrayList<>();
        ArrayList<Venue>  venuelst = new ArrayList<>();
        File fileModule = new File(filepathModule);
        File fileVenue = new File(filepathVenue);

        if(fileModule.exists() && fileVenue.exists()) {
            modulelst = (ArrayList<Module>) schedule.myreadFile(filepathModule,fileModule,moduleList);
            venuelst = (ArrayList<Venue>) schedule.myreadFile(filepathVenue,fileVenue,venueList);
            Collections.sort(modulelst);
            Collections.sort(venuelst);

            schedule.makeTimeTable();
            schedule.scheduling(modulelst,venuelst);
            schedule.printTimetable("timetable");
        } else {
            System.out.println("Error: file not found");
        }
    }



}
