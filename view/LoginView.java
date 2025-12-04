package view;

import dao.UserDAO;
import model.User;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;

public class LoginView extends JFrame {

    private JTextField usernameField;
    private JPasswordField passwordField;
    private UserDAO userDAO;
    private JComboBox<String> roleSelector;

    public LoginView() {
        super("MediTrack - Secure Login");
        this.userDAO = new UserDAO();
        
        // Настройка окна
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(400, 300);
        setLocationRelativeTo(null); // Центрирование экрана
        setLayout(new BorderLayout());

        // == Логотип / Заголовок ==
        JLabel titleLabel = new JLabel("MediTrack System", SwingConstants.CENTER);
        titleLabel.setFont(new Font("Arial", Font.BOLD, 24));
        titleLabel.setBorder(BorderFactory.createEmptyBorder(20, 0, 20, 0));
        add(titleLabel, BorderLayout.NORTH);

        // == Форма ввода (Центр) ==
        JPanel formPanel = new JPanel(new GridLayout(3, 2, 10, 10));
        formPanel.setBorder(BorderFactory.createEmptyBorder(20, 40, 20, 40));

        formPanel.add(new JLabel("Username:"));
        usernameField = new JTextField();
        formPanel.add(usernameField);

        formPanel.add(new JLabel("Password:"));
        passwordField = new JPasswordField();
        formPanel.add(passwordField);
        
        formPanel.add(new JLabel("Role:"));
        String[] roles = {"PATIENT", "DOCTOR"};
        roleSelector = new JComboBox<>(roles);
        formPanel.add(roleSelector);

        add(formPanel, BorderLayout.CENTER);

        // == Кнопки (Низ) ==
        JPanel buttonPanel = new JPanel(new FlowLayout());
        
        JButton loginButton = new JButton("Login");
        JButton registerButton = new JButton("Register / Sign Up");

        // Логика кнопки Login
        loginButton.addActionListener((ActionEvent e) -> handleLogin());

        // Логика кнопки Register
        registerButton.addActionListener((ActionEvent e) -> handleRegistration());

        buttonPanel.add(registerButton);
        buttonPanel.add(loginButton);
        add(buttonPanel, BorderLayout.SOUTH);
        
        setVisible(true);
    }

    private void handleLogin() {
        String user = usernameField.getText();
        String pass = new String(passwordField.getPassword());

        // Вызываем наш DAO (он сам захэширует пароль и сверит)
        User authenticatedUser = userDAO.login(user, pass);

        if (authenticatedUser != null) {
            JOptionPane.showMessageDialog(this, "Welcome back, " + authenticatedUser.getUsername() + "!");
            dispose(); // Закрываем окно логина
            authenticatedUser.openDashboard(); // Полиморфизм: открывается нужное окно
        } else {
            JOptionPane.showMessageDialog(this, "Invalid credentials!", "Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    private void handleRegistration() {
        String user = usernameField.getText();
        String pass = new String(passwordField.getPassword());
        String selectedRole = (String) roleSelector.getSelectedItem(); // Берем роль из списка

        if (user.isEmpty() || pass.isEmpty()) {
            JOptionPane.showMessageDialog(this, "Please fill fields.");
            return;
        }

        // Передаем выбранную роль в DAO
        boolean success = userDAO.registerUser(user, pass, selectedRole);

        if (success) {
            JOptionPane.showMessageDialog(this, "Registered as " + selectedRole + "!");
        } else {
            JOptionPane.showMessageDialog(this, "Registration failed.");
       }
    }
}