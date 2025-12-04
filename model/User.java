package model;

/**
 * Абстрактная модель пользователя.
 * Демонстрирует принципы Инкапсуляции и Абстракции.
 */
public abstract class User {
    private int id;
    private String username;
    private String passwordHash;
    private String role; // "PATIENT", "DOCTOR", "ADMIN"

    // Конструктор
    public User(int id, String username, String passwordHash, String role) {
        this.id = id;
        this.username = username;
        this.passwordHash = passwordHash;
        this.role = role;
    }

    // Геттеры (Setters не нужны, если мы не планируем менять ID или роль на лету)
    public int getId() { return id; }
    public String getUsername() { return username; }
    public String getPasswordHash() { return passwordHash; }
    public String getRole() { return role; }

    /**
     * Абстрактный метод.
     * Каждый наследник (Доктор/Пациент) должен реализовать свою логику,
     * какую панель управления ему открывать.
     * Это демонстрация ПОЛИМОРФИЗМА.
     */
    public abstract void openDashboard();
    
    @Override
    public String toString() {
        return "User{" + "id=" + id + ", role='" + role + '\'' + '}';
    }
}