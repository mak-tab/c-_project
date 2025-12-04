package dao;

import model.Doctor;
import model.Patient;
import model.User;
// Эти классы мы создадим чуть позже, пока просто держим в уме
// import com.meditrack.model.Patient;
// import com.meditrack.model.Doctor;
import util.DatabaseConnection;
import util.SecurityUtils;

import java.sql.*;

public class UserDAO {

    /**
     * Регистрация нового пользователя.
     * Принимает сырой пароль, но сохраняет ХЭШ.
     */
    public boolean registerUser(String username, String rawPassword, String role) {
        String sql = "INSERT INTO users(username, password_hash, role) VALUES(?, ?, ?)";

        try (Connection conn = DatabaseConnection.getConnection();
             PreparedStatement pstmt = conn.prepareStatement(sql)) {

            // ХЭШИРУЕМ пароль ПЕРЕД сохранением!
            String secureHash = SecurityUtils.hashPassword(rawPassword);

            pstmt.setString(1, username);
            pstmt.setString(2, secureHash); // В БД летит только хэш
            pstmt.setString(3, role);

            pstmt.executeUpdate();
            return true;

        } catch (SQLException e) {
            System.err.println("[Register Error] " + e.getMessage());
            return false;
        }
    }

    /**
     * Авторизация пользователя.
     * Проверяет, совпадает ли хэш введенного пароля с хэшем в БД.
     */
    public User login(String username, String rawPassword) {
        String sql = "SELECT * FROM users WHERE username = ?";

        try (Connection conn = DatabaseConnection.getConnection();
             PreparedStatement pstmt = conn.prepareStatement(sql)) {

            pstmt.setString(1, username);
            ResultSet rs = pstmt.executeQuery();

            if (rs.next()) {
                String storedHash = rs.getString("password_hash");
                // Хэшируем то, что ввел юзер сейчас, и сверяем с тем, что в базе
                if (storedHash.equals(SecurityUtils.hashPassword(rawPassword))) {
                    return mapToUser(rs);
                }
            }

        } catch (SQLException e) {
            System.err.println("[Login Error] " + e.getMessage());
        }
        return null; // Пользователь не найден или пароль неверный
    }

    // Вспомогательный метод маппинга (превращает строку таблицы в Объект Java)
    // ОБНОВЛЕННЫЙ МЕТОД
    private User mapToUser(ResultSet rs) throws SQLException {
        int id = rs.getInt("id");
        String username = rs.getString("username");
        String hash = rs.getString("password_hash");
        String role = rs.getString("role");

        // ПОЛИМОРФИЗМ В ДЕЙСТВИИ:
        // В зависимости от строки в БД создаем РАЗНЫЕ объекты
        if ("PATIENT".equalsIgnoreCase(role)) {
            return new Patient(id, username, hash);
        } else if ("DOCTOR".equalsIgnoreCase(role)) {
            return new Doctor(id, username, hash);
        } else {
            // Если роль неизвестна или ADMIN (реализуем позже если нужно)
            return null; 
        }
    }
}