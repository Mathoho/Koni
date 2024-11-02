package org.example;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.fail;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class NameTest {
    @Test
    public void testValidname() {
        Schedule schedule = new Schedule();
        boolean result = schedule.isvalidName("Mathematics");
        assertEquals(true, result);

        boolean result2 = schedule.isvalidName("Konanani Mathoho");
        assertEquals(true, result2);

        boolean result3 = schedule.isvalidName("Jhon-Craig Smith has a very good day in the park with jhon");
        assertEquals(false, result3);

        boolean result4 = schedule.isvalidName("12345678 8785765674 653674672 34786o8723478");
        assertEquals(false, result4);

        boolean result5 = schedule.isvalidName("12345678");
        assertEquals(false, result5);
    }

    @Test
    public void testValidday() {
        Schedule schedule = new Schedule();
        boolean result = schedule.validDay("Mathematics");
        assertEquals(false, result);
        boolean result2 = schedule.validDay("Monday");
        assertEquals(true, result2);
        boolean result3 = schedule.validDay("tuesday");
        assertEquals(true, result3);
    }

    @Test
    public void testaddTimeOccupied() {
        Venue venue = new Venue("Room 234",300,false,"");

        int result = venue.getTimeOccupied().size();
        assertEquals(false,isposInteger(result));

        venue.addTimeOccupied("Monday","11:00 AM - 12:00 PM");
        int result2 = venue.getTimeOccupied().size();
        assertEquals(true,isposInteger(result2));
    }

    private static boolean isposInteger(int num){
        if (num > 0){
            return true;
        }else {
            return false;
        }
    }
    @Test
    public void testisdayTimeTaken() {
        Venue venue = new Venue("Room 234",300,false,"");

        boolean result = venue.isdayTimeTaken("Monday","11:00 AM - 12:00 PM");
        assertEquals(false,result);

        venue.addTimeOccupied("Monday","11:00 AM - 12:00 PM");
        boolean result2 = venue.isdayTimeTaken("Monday","11:00 AM - 12:00 PM");
        assertEquals(true,result2);
    }

    @Test
    public void testsetOccupied() {
        Venue venue = new Venue("Room 234",300,false,"");

        boolean result = venue.getIsOccupied();
        assertEquals(false,result);

        venue.setOccupied(true);
        boolean result2 = venue.getIsOccupied();
        assertEquals(true,result2);
    }



    @Test
    public void testgetNxtlectTime() {
        Module module = new Module("Mathematics",200,"Prof Mike","Monday","11:00 AM - 13:00 PM","");

        String result = module.getNxtlectTime("09:00 AM - 10:00 AM");
        assertEquals("Lecturer is not available", result);

        String result2 = module.getNxtlectTime("14:00 AM - 15:00 PM");
        assertEquals("Lecturer is not available", result2);

        String result3 = module.getNxtlectTime("11:00 AM - 12:00 PM");
        assertEquals("12:00 PM - 13:00 PM", result3);

        module.setLectureTime("09:00 AM - 16:00 PM");
        String result4 = module.getNxtlectTime("12:00 AM - 13:00 PM");
        assertEquals("13:00 PM - 14:00 PM", result4);
    }

    @Test
    public void testgetTheHour() {
        Module module = new Module("Mathematics",200,"Prof Mike","Monday","11:00 AM - 13:00 PM","");

        String result = module.getTheHour("11:00 AM - 13:00 PM");
        assertEquals("11:00 AM - 12:00 PM", result);

        String result3 = module.getTheHour("11:00 AM - 12:00 PM");
        assertEquals("11:00 AM - 12:00 PM", result3);
    }

    @Test
    public void testCompatibility() {

        Schedule schedule = new Schedule();
        Module module = new Module("Mathematics",200,"Prof Mike","Monday","11:00 AM - 13:00 PM","");
        Venue venue = new Venue("Room 234",300,false,"");
        boolean result = schedule.compatibility(module,venue);
        assertEquals(true, result);

        Module module2 = new Module("Mathematics",500,"Prof Mike","Monday","11:00 AM - 13:00 PM","");
        Venue venue2 = new Venue("Room 234",300,false,"");
        venue.addTimeOccupied("Monday","11:00 AM - 12:00 PM");
        boolean result2 = schedule.compatibility(module2,venue2);
        assertEquals(false, result2);

        Module module3 = new Module("Mathematics",100,"Prof Mike","Monday","11:00 AM - 13:00 PM","");
        Venue venue3 = new Venue("Room 234",300,true,"");
        boolean result3 = schedule.compatibility(module3,venue3);
        assertEquals(false, result3);
    }

    //TODO test for submission 2
    @Test
    public void testIsModuleFound(){
        Module module = new Module("Mathematics",200,"Prof Mike","Monday","11:00 AM - 13:00 PM","");
        boolean result = module.isModuleFound("Mathematics");
        assertEquals(true, result);
    }

    @Test 
    public void testMakeTimeTable(){
        Schedule schedule = new Schedule();
        schedule.makeTimeTable();
        String[][] timetable = schedule.getTimeTable();
        String result = timetable[0][0];
        String exp = "DAY/TIME";
        assertEquals(exp, result);

        String result2 = timetable[0][1];
        String exp2 = "Monday";
        assertEquals(exp2, result2);

        String result3 = timetable[0][4];
        String exp3 = "Thursday";
        assertEquals(exp3, result3);

        String result4 = timetable[0][2];
        String exp4 = "Tuesday";
        assertEquals(exp4, result4);
    }

    @Test
    public void testGetcolumn(){
        Schedule schedule = new Schedule();
        schedule.makeTimeTable();
        int result = schedule.getColumn("Monday");
        assertEquals(1, result);

        int result2 = schedule.getColumn("friday");
        assertEquals(5, result2);

        int result3 = schedule.getColumn("WedNesDay");
        assertEquals(3, result3);

        int result4 = schedule.getColumn("not a day");
        assertEquals(-1, result4);
    }

    @Test 
    public void testUpdateTable(){
        Schedule schedule = new Schedule();
        schedule.makeTimeTable();
        schedule.updateTimetable("A306A", "09:00", "Tuesday", "Computer Science 344", "Luke William");
        schedule.updateTimetable("A406B", "11:00", "Friday", "Applied Mathematics 364", "Michael Schofield");
        String[][] timetable = schedule.getTimeTable();
        String result = timetable[2][2];
        String exp = "venue:A306A:module:Computer Science 344:Lecturer:Luke William";
        assertEquals(exp, result);

        
        String result2 = timetable[4][5];
        String exp2 = "venue:A406B:module:Applied Mathematics 364:Lecturer:Michael Schofield";
        assertEquals(exp2, result2);

    }
    @Test
    public void testPrintTimetable() {
        Schedule schedule = new Schedule();
        schedule.makeTimeTable();
        schedule.updateTimetable("A306A", "09:00", "Tuesday", "Computer Science 344", "jonathan");
        schedule.printTimetable("timetable");
        String filePath = "./Output/timetable.txt";

        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            String line =  reader.readLine();
            boolean result = schedule.validDay(line);

            assertEquals(true, result);
        } catch (IOException e) {
            fail("An error occurred while reading the file: " + e.getMessage());
        }
}



}
