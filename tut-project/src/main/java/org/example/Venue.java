package org.example;

import java.util.ArrayList;
import java.util.HashMap;

public class Venue implements Comparable<Venue> {

    public  ArrayList<String> lstVenueNames = new ArrayList<>();
    //public  ArrayList<String> timesOccupied = new ArrayList<>();
    HashMap<String, ArrayList<String>> timesOccupied = new HashMap<>();
    public int sizeVenueList = 0;
    String roomNum;
    int venueCapacity;
    boolean occupied;
    String moduleName;

    public Venue(String rmNm, int size, boolean ocpd, String modNm){
        this.roomNum = rmNm;
        this.venueCapacity = size;
        this.occupied = ocpd;
        this.moduleName = modNm;

        this.lstVenueNames.add(rmNm);
        this.sizeVenueList+=1;
    }
    //getters
    public int getVenueCapacity() {
        return venueCapacity;
    }

    public String getRoomNum() {
        return roomNum;
    }

    public boolean getIsOccupied() {
        return occupied;
    }

    public String getModuleName() {
        return moduleName;
    }

    public void setModuleName(String moduleName) {
        this.moduleName = moduleName;
    }

    //setters
    public void setOccupied(boolean occupied) {
        this.occupied = occupied;
    }

    public void setRoomNum(String roomNum) {
        this.roomNum = roomNum;
    }

    /*
    public void setSize(int size) {
        this.venueCapacity = size;
    }*/

    public int getSizeVenueList() {
        return sizeVenueList;
    }

    public ArrayList getLstVenueNames() {
        return lstVenueNames;
    }

    public  void addVenueName(String moduleName) {
        this.lstVenueNames.add(moduleName);
        this.sizeVenueList+=1;
    }

    public boolean isVenueFound(String venueName){
        if(this.lstVenueNames.contains(venueName)){
            return true;
        }
        return false;
    }

    public void addTimeOccupied(String day,String time) {
        if(!getTimeOccupied().isEmpty()) {
            ArrayList<String> temp;
            if (this.timesOccupied.containsKey(day)) {
                temp = this.timesOccupied.get(day);
            } else {
                temp = new ArrayList<>();
            }
            temp.add(time);
            this.timesOccupied.put(day, temp);
        }else {
            ArrayList<String> temp = new ArrayList<>();
            temp.add(time);
            getTimeOccupied().put(day,temp);
        }
    }

    public  HashMap<String, ArrayList<String>> getTimeOccupied() {
        return this.timesOccupied;
    }

    public boolean isdayTimeTaken(String day, String time){
        if(!getTimeOccupied().isEmpty()) {
            if(getTimeOccupied().containsKey(day)) {
                ArrayList<String> dayOccupation = this.getTimeOccupied().get(day);//definately not empty since i hv key
                    for (String x : dayOccupation) {
                        if (x.equals(time)) {
                            return true;
                        }
                    }
            }else {

                return false;
            }
        }
        addTimeOccupied(day,time);
        return false;
    }
    @Override
    public int compareTo(Venue otherVenue) {
        return Integer.compare(this.venueCapacity, otherVenue.venueCapacity);
    }

    public void printVenue(){
        System.out.println("roomNum: "+ this.roomNum );
        System.out.println("venueCapacity: "+ this.venueCapacity );
        System.out.println("occupied: "+ this.occupied );
        System.out.println("moduleName: "+ this.moduleName );
    }
}
