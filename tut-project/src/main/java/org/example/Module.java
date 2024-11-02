package org.example;
import java.time.LocalTime;
import java.util.ArrayList;
import java.util.Comparator;

public  class Module implements Comparable<Module> {
    public ArrayList lstModuleNames = new ArrayList<>();
    public int sizeModuleList = 0;
    String lectureTime;
    String dayOfWeek;
    String moduleName;
    String lectureName;
    int numStudents;
    String roomNum;

    public  Module(String modName,int numStd, String lecturer,String day ,String time,String rmNm){
        this.lectureTime = time;
        this.moduleName = modName;
        this.lectureName = lecturer;
        this.numStudents = numStd;
        this.roomNum = rmNm;
        this.dayOfWeek = day;

        this.lstModuleNames.add(modName);
        this.sizeModuleList+=1;
        
    }

    public void setModuleName(String moduleName) {
        this.moduleName = moduleName;
    }

    public String getModuleName() {
        return moduleName;
    }

    public void setRoomNum(String roomNum) {
        this.roomNum = roomNum;
    }

    public String getRoomNum() {
        return roomNum;
    }

    public int getNumStudents() {
        return numStudents;
    }

    public void setNumStudents(int numStudents) {
        this.numStudents = numStudents;
    }

    public String getLectureTime() {
        return this.lectureTime;
    }

    public void setLectureTime(String lectureTime) {
        this.lectureTime = lectureTime;
    }

    public String getLectureName() {
        return lectureName;
    }

    public void setLectureName(String lectureName) {
        this.lectureName = lectureName;
    }

    public ArrayList getLstModuleNames() {
        return lstModuleNames;
    }

    public int getSizeModuleList() {
        return sizeModuleList;
    }

    public  void addModuleName(String moduleName) {
        this.lstModuleNames.add(moduleName);
        this.sizeModuleList+=1;
    }

    public  boolean isModuleFound(String moduleName) {
        if(this.lstModuleNames.contains(moduleName)){
            return true;
        }
        return false;
    }

    public String getDayOfWeek(){
        return this.dayOfWeek;
    }

    @Override
    public int compareTo(Module otherModule) {
        return Integer.compare(this.numStudents, otherModule.numStudents);
    }

    public static  void printarr(String[] arr){
        for (int i = 0; i < arr.length; i++){
            System.out.print(arr[i] + "-");
        }
        System.out.println();
    }

    public  String getNxtlectTime(String currentTime){
        String[] data = currentTime.split("-");
        String firstPart = data[1].strip().concat(" - ");

        String[] second = firstPart.split(":");
        int numFr = Integer.parseInt(second[0].strip());
        int numSec = numFr + 1;

        String[] dataCheck = this.getLectureTime().split("-");
        String a = dataCheck[0].strip();
        String[] d = a.split(":");
        int b =  Integer.parseInt(d[0].strip());
        if(b>numFr){
            return "Lecturer is not available";
        }

        String firstPartChck = dataCheck[1].strip();
        String[] secondCheck = firstPartChck.split(":");
        int chk =  Integer.parseInt(secondCheck[0].strip());


        if(numFr <= chk) {
            String sec_part;
            if (numSec < 12) {
                sec_part = numSec + ":00 AM ";

            } else {
                sec_part = numSec + ":00 PM";

            }
            return firstPart.concat(sec_part);
        }
        return  "Lecturer is not available";
    }

    public String getTheHour(String fullTime){
        String[] data = fullTime.split("-");
        String firstPart = data[0].strip();
        String[] second = firstPart.split(":");

        String secPartStp = data[1].strip();
        String[] secondStp = secPartStp.split(":");
        int numSecStop = Integer.parseInt(secondStp[0].strip());

        int numSec = Integer.parseInt(second[0].strip()) + 1;
        String secPart;
        if(numSec <= numSecStop){
            if (numSec < 12) {
                secPart =  " - "+numSec + ":00 AM ";
            }else {
                secPart = " - "+ numSec + ":00 PM";
            }
            return firstPart.concat(secPart);
        }
        return null;
    }

    public void printModule(){
        System.out.println("LectureTime: " + this.lectureTime);
        System.out.println("moduleName: " + this.moduleName);
        System.out.println("numStudents: " + this.numStudents);
        System.out.println("roomNum: " + this.roomNum);
        System.out.println("dayOfWeek: " + this.dayOfWeek);
    }

}
