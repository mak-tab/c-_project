package dao;

import util.DatabaseConnection;
import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class HistoryDAO {

    // Сохранение диагноза
    public void saveDiagnosis(String patientName, String diseaseName) {
        String sql = "INSERT INTO history(patient_username, disease_name) VALUES(?, ?)";
        try (Connection conn = DatabaseConnection.getConnection();
             PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, patientName);
            pstmt.setString(2, diseaseName);
            pstmt.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    // Чтение всей истории (для Доктора)
    public List<String[]> getAllRecords() {
        List<String[]> records = new ArrayList<>();
        String sql = "SELECT patient_username, disease_name, timestamp FROM history ORDER BY timestamp DESC";
        
        try (Connection conn = DatabaseConnection.getConnection();
             Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(sql)) {
            
            while (rs.next()) {
                records.add(new String[]{
                    rs.getString("patient_username"),
                    rs.getString("disease_name"),
                    rs.getString("timestamp")
                });
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return records;
    }
}