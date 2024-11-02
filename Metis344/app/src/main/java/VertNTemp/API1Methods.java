package VertNTemp;
import java.sql.*;

import io.vertx.core.json.JsonObject;
 
/**
 * Contains all customer functions
 * 
 * @author Hishaam Mahomed
 */
public class API1Methods implements API1Interface {
    private Connection conn;
    private final String table_name = "api1table";
 
    /**
     * Connects to the customer database
     * 
     * @param dbname name of database
     * @param user username of owner
     * @param password password of owner
     */
    public void connect_to_db(String dbname, String user, String password) {
        try {
            Class.forName("org.postgresql.Driver");
            conn = DriverManager.getConnection("jdbc:postgresql://localhost:5432/" + dbname, user, password);
 
            if (conn != null) {
                System.out.println("Connection to API1 Database established");
            } else {
                System.out.println("Connection to API1 Database failed");
            }
        } catch (Exception e) {
            //System.out.println(e);
        }
    }
 
    /**
     * Adds new customer to the database table
     * 
     * @param user_id customer ID number within business records
     * @param full_name name of customer
     * @param identification_number personal ID number of customer
     * @param pan personal account number of customer
     * @param current_balance current bank balance of customer
     * @param email_address email address of customer
     * @param contact_number phone number of customer
     * @param address residential address of customer
     * @param date_of_birth date of birth of customer
     */
    public void new_customer(int user_id, String full_name, String identification_number, String pan, int current_balance, String email_address, String contact_number, String address, String date_of_birth) {
        Statement statement;
        try {
            String query = String.format("INSERT INTO %s(\"User ID\", \"Full Name\", \"ID\", \"PAN\", \"Current Balance\", \"Email Address\", \"Contact Number\", \"Address\", \"Date Of Birth\") VALUES('%d', '%s', '%s', '%s', '%d', '%s', '%s', '%s', '%s');",
                    table_name, user_id, full_name, identification_number, pan, current_balance, email_address, contact_number, address, date_of_birth);
 
            statement = conn.createStatement();
            statement.executeUpdate(query);
 
        } catch (Exception e) {
            //System.out.println(e);
        }
    }
 
    /**
     * Gets the current bank balance of specified custome
     * 
     * @param pan personal account number of customer
     * @return current bank balance
     */
    public double get_current_balance(String pan) {
        Statement statement;
        ResultSet rs;
        double currentBalance = 0.0;
 
        try {
            String query = String.format("SELECT \"Current Balance\" FROM %s WHERE \"PAN\" = '%s'", table_name, pan);
            statement = conn.createStatement();
            rs = statement.executeQuery(query);
 
            if (rs.next()) {
                currentBalance = rs.getDouble("Current Balance");
            }
        } catch (Exception e) {
            //System.out.println(e.getMessage());
            return -1;
        }
 
        return currentBalance;
    }
 
    /**
     * Processes the customers side of a transaction 
     * 
     * @param pan personal account number of customer
     * @param amount transaction amount
     * @param transaction_type type of transaction (payment or reversal)
     * @return status of transaction
     */
    public String transaction(String pan, double amount, String transaction_type) {
        try {
            double current_balance = get_current_balance(pan);
            if (current_balance == -1) {
                return "ERROR";
            }
            if (current_balance < amount && transaction_type == "PAYMENT") {
                return "Insufficient funds";
            }
            double new_balance;
 
            switch (transaction_type) {
                case "PAYMENT":
                    new_balance = current_balance - amount;
                    break;
                case "REVERSAL":
                    new_balance = current_balance + amount;
                    break;
                default:
                    System.out.println("Invalid transaction type");
                    return "INVALID TRANSACTION TYPE";
            }
 
            String sql = "UPDATE " + table_name + " SET \"Current Balance\" = ? WHERE \"PAN\" = ?";
            PreparedStatement preparedStatement = conn.prepareStatement(sql);
            preparedStatement.setDouble(1, new_balance);
            preparedStatement.setString(2, pan);
 
            int rowsUpdated = preparedStatement.executeUpdate();
            if (rowsUpdated > 0) {
                System.out.println("Transaction successful. New balance: " + new_balance);
                return "SUCCESS";
            } else {
                System.out.println("Transaction failed.");
                return "FAILED";
            }
        } catch (Exception e) {
            //System.out.println("Error performing transaction: " + e.getMessage());
            return "ERROR";
        }
    }
 
    /**
     * Gets the customer ID number according to business records
     * @param pan personal account number of customer
     * @return the customer ID number
     */
    public int get_user_id(String pan) {
        Statement statement;
        ResultSet rs;
 
        try {
            String query = String.format("SELECT \"User ID\" FROM %s WHERE \"PAN\" = '%s'", table_name, pan);
            statement = conn.createStatement();
            rs = statement.executeQuery(query);
            if (rs.next()) {
                int user_id = rs.getInt("User ID");
                return user_id;
            }
            statement.close();
        } catch (Exception e) {
            //System.out.println(e);
            return -1;
        }
 
        return -1;
    }

    //new implementation for API1
    public String deleteUser(int userId) {
        Statement statement;
        try {
            String query = String.format("DELETE FROM %s WHERE \"User ID\" = %d;", table_name, userId);
            statement = conn.createStatement();
            statement.executeUpdate(query);
            statement.close();
            return "User deleted successfully";
        } catch (Exception e) {
            //System.out.println(e);
            return "Failed to Delete user";
        }
    }
        /**
     * Gets the customer ID number according to business records
     * @param pan personal account number of customer
     * @return the customer ID number
     */
    public JsonObject get_user(String pan) {
        Statement statement;
        ResultSet rs;
        JsonObject userJason = new JsonObject();
        try {
            String query = String.format("SELECT * FROM %s WHERE \"PAN\" = '%s'", table_name, pan);
            statement = conn.createStatement();
            rs = statement.executeQuery(query);
            if (rs != null) {
                while (rs.next()) {
                    int userId = rs.getInt("User ID");
                    String fullName = rs.getString("Full Name");
                    String id = rs.getString("ID");
                    double balance = rs.getDouble("Current Balance");
                    String email = rs.getString("Email Address");
                    String contactNum = rs.getString("Contact Number");
                    String address = rs.getString("Address");
                    String dateOfBirth = rs.getString("Date Of Birth");

                    // Add the information to the JSON object
                    userJason.put("UserId", userId);
                    userJason.put("FullName", fullName);
                    userJason.put("ID", id);
                    userJason.put("PAN", pan);
                    userJason.put("Balance", balance);
                    userJason.put("EmailAddress", email);
                    userJason.put("ContactNumber", contactNum);
                    userJason.put("Address", address);
                    userJason.put("DateOfBirth", dateOfBirth);

                }
                return userJason;
            }
            userJason.put("Error","Failed to delete a user");
            return userJason;
        } catch (Exception e) {
            userJason.put("Error",e.toString());
            return userJason;
        }
    }

    public String updatePhoneNumber(String prevPhoneNumber, String newPhoneNumber) {
        try {
            String sql = "UPDATE " + table_name + " SET \"Contact Number\" = ? WHERE \"Contact Number\" = ?";
            PreparedStatement preparedStatement = conn.prepareStatement(sql);
            preparedStatement.setString(1, newPhoneNumber);
            preparedStatement.setString(2, prevPhoneNumber);
 
            int rowsUpdated = preparedStatement.executeUpdate();
            if (rowsUpdated > 0) {
                return "SUCCESS";
            } else {
                return "FAILED";
            }
        } catch (Exception e) {
            e.printStackTrace();
            return "ERROR";
        }
    }

    //TODO remove this
      public static void main(String[] args) {
    System.out.println("in the main method now");
    API1Methods db1 = new API1Methods();
        db1.connect_to_db("api1databasek", "konanani", "123456");
        // String val = db1.deleteUser(1); //TODO tested works
        // System.out.println(val + " of deleting the user");
        // if (val != null) {
        //     System.out.println("they have everything now");
        // }else{
        //     System.out.println("they do not have ahything");
        // }
        // JsonObject user = db1.get_user("FGHIJ5678K");  //TODO tested works
        // System.out.println(user.toString());
        String rst = db1.updatePhoneNumber("+1-555-987-6543", "09997NEW"); //TODO tested and it works
        System.out.println(rst);
   }
}