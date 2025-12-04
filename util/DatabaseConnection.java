package util;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;

/**
 * Класс для управления подключением к базе данных SQLite.
 * Реализует паттерн Singleton.
 */
public class DatabaseConnection {

    // Статическая переменная для хранения единственного экземпляра
    private static Connection connection = null;

    // Константа с именем файла БД
    private static final String URL = "jdbc:sqlite:meditrack.db";

    // Приватный конструктор, чтобы никто не мог создать экземпляр через new DatabaseConnection()
    private DatabaseConnection() { }

    /**
     * Метод для получения подключения.
     * Если подключения нет - создает. Если есть - возвращает существующее.
     */
    public static Connection getConnection() {
        try {
            if (connection == null || connection.isClosed()) {
                // Загружаем драйвер (обязательно для старых версий Java/JDBC)
                Class.forName("org.sqlite.JDBC"); 
                connection = DriverManager.getConnection(URL);
                System.out.println("[DB] Connection established.");
                
                // Сразу инициализируем таблицы при первом запуске
                initializeDatabase();
            }
        } catch (ClassNotFoundException | SQLException e) {
            System.err.println("[DB Error] Failed to connect: " + e.getMessage());
            // В реальном Enterprise здесь был бы Logger, но для проекта System.err достаточно
        }
        return connection;
    }

    /**
     * Создание таблиц, если они не существуют (DDL).
     */
    private static void initializeDatabase() {
        // SQL запрос на создание таблицы пользователей
        String createUsersTable = "CREATE TABLE IF NOT EXISTS users ("
                + "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                + "username TEXT NOT NULL UNIQUE,"
                + "password_hash TEXT NOT NULL,"
                + "role TEXT NOT NULL,"
                + "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
                + ");";

        // SQL запрос на создание таблицы истории
        String createHistoryTable = "CREATE TABLE IF NOT EXISTS history ("
                + "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                + "patient_username TEXT NOT NULL,"
                + "disease_name TEXT NOT NULL,"
                + "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP"
                + ");";

        // Обрати внимание: оба запроса выполняются ВНУТРИ одного блока try
        try (Statement stmt = connection.createStatement()) {
            // 1. Создаем таблицу юзеров
            stmt.execute(createUsersTable);
            
            // 2. Создаем таблицу истории (ВНУТРИ этого же блока)
            stmt.execute(createHistoryTable);
            
        } catch (SQLException e) {
            System.err.println("[DB Error] Table creation failed: " + e.getMessage());
        }
    }
}