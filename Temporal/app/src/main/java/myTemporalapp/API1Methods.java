package myTemporalapp;

import java.sql.*;
 
/**
 * Contains all customer functions
 * 
 * @author Hishaam Mahomed
 */
public class API1Methods implements API1MethodsInfrc{
    private Connection conn;
    private final String table_name = "api1table";
 
    /**
     * Connects to the customer database
     * 
     * @param dbname name of database
     * @param user username of owner
     * @param password password of owner
     */
    @Override
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
            System.out.println(e);
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
    @Override
    public void new_customer(int user_id, String full_name, String identification_number, String pan, int current_balance, String email_address, String contact_number, String address, String date_of_birth) {
        Statement statement;
        try {
            String query = String.format("INSERT INTO %s(\"User ID\", \"Full Name\", \"ID\", \"PAN\", \"Current Balance\", \"Email Address\", \"Contact Number\", \"Address\", \"Date Of Birth\") VALUES('%d', '%s', '%s', '%s', '%d', '%s', '%s', '%s', '%s');",
                    table_name, user_id, full_name, identification_number, pan, current_balance, email_address, contact_number, address, date_of_birth);
 
            statement = conn.createStatement();
            System.out.println("Statement: " + statement);
            statement.executeUpdate(query);
 
            System.out.println("Row inserted");
 
        } catch (Exception e) {
 
            System.out.println(e);
        }
    }
 
    /**
     * Gets the current bank balance of specified custome
     * 
     * @param pan personal account number of customer
     * @return current bank balance
     */
    @Override
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
            System.out.println(e.getMessage());
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
    @Override
    public String transaction(String pan, double amount, String transaction_type) {
        System.out.println("made it to api1methods");
        try {
            double current_balance = get_current_balance(pan);
            if (current_balance == -1) {
                System.out.println("in -1");
                return "ERROR";
            }
            if (current_balance < amount && transaction_type == "PAYMENT") {
                System.out.println("Insufficient funds");
                return "Insufficient funds";
            }
            double new_balance;
 
            switch (transaction_type) {
                case "PAYMENT":
                System.out.println("paments in api1");
                    new_balance = current_balance - amount;
                    break;
                case "REVERSAL":
                    new_balance = current_balance + amount;
                    break;
                default:
                    System.out.println("Invalid transaction type");
                    return "INVALID TRANSACTION TYPE";
            }
            System.out.println("update the qeury");
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
            System.out.println("Error performing transaction: " + e.getMessage());
            return "ERROR";
        }
    }
 
    /**
     * Gets the customer ID number according to business records
     * @param pan personal account number of customer
     * @return the customer ID number
     */
    @Override
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
        } catch (Exception e) {
            System.out.println(e);
            return -1;
        }
 
        return -1;
    }
}