package view;

import service.GeminiService; // Мы создадим это ниже
import dao.HistoryDAO;
import javax.swing.*;
import java.awt.*;

public class PatientDashboard extends JFrame {

    private JTextArea chatArea;
    private JTextField inputField;
    private JButton sendButton;
    private GeminiService aiService;
    private String username;

    public PatientDashboard(String username) {
        super("MediTrack AI - Patient: " + username);
        this.username = username;
        this.aiService = new GeminiService(); // Инициализация ИИ сервиса

        setSize(600, 700);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        // == ЦВЕТОВАЯ ПАЛИТРА (Dark Mode) ==
        Color darkBg = new Color(40, 44, 52);       // Темно-серый фон
        Color darkerBg = new Color(33, 37, 43);     // Еще темнее для инпутов
        Color accentColor = new Color(97, 175, 239); // Голубой акцент
        Color textColor = new Color(220, 223, 228); // Светлый текст

        // == ЦЕНТР (Чат) ==
        chatArea = new JTextArea();
        chatArea.setEditable(false);
        chatArea.setBackground(darkBg);
        chatArea.setForeground(textColor);
        chatArea.setFont(new Font("Segoe UI", Font.PLAIN, 16));
        chatArea.setLineWrap(true);
        chatArea.setWrapStyleWord(true);
        chatArea.setMargin(new Insets(10, 10, 10, 10));

        JScrollPane scrollPane = new JScrollPane(chatArea);
        scrollPane.setBorder(null);
        add(scrollPane, BorderLayout.CENTER);

        // Приветственное сообщение
        appendMessage("AI Assistant", "Hello! Describe your symptoms, and I will analyze them.");

        // == НИЗ (Ввод) ==
        JPanel inputPanel = new JPanel(new BorderLayout(10, 10));
        inputPanel.setBackground(darkerBg);
        inputPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        inputField = new JTextField();
        inputField.setBackground(darkBg);
        inputField.setForeground(textColor);
        inputField.setCaretColor(Color.WHITE);
        inputField.setFont(new Font("Segoe UI", Font.PLAIN, 14));
        inputField.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        sendButton = new JButton("Send");
        sendButton.setBackground(accentColor);
        sendButton.setForeground(Color.WHITE); // Белый текст на кнопке
        sendButton.setFont(new Font("Segoe UI", Font.BOLD, 14));
        sendButton.setFocusPainted(false);
        sendButton.setBorder(BorderFactory.createEmptyBorder(10, 20, 10, 20));
        sendButton.setCursor(new Cursor(Cursor.HAND_CURSOR));

        // Логика отправки по Enter
        inputField.addActionListener(e -> sendMessage());
        sendButton.addActionListener(e -> sendMessage());

        inputPanel.add(inputField, BorderLayout.CENTER);
        inputPanel.add(sendButton, BorderLayout.EAST);

        add(inputPanel, BorderLayout.SOUTH);

        setVisible(true);
    }

    private void appendMessage(String sender, String text) {
        chatArea.append("[" + sender + "]: " + text + "\n\n");
        chatArea.setCaretPosition(chatArea.getDocument().getLength()); // Автоскролл вниз
    }

    private void sendMessage() {
        String text = inputField.getText().trim();
        if (text.isEmpty()) return;

        // 1. Показываем сообщение пользователя
        appendMessage("You", text);
        inputField.setText("");
        inputField.setEnabled(false); // Блокируем ввод пока ждем ответа
        sendButton.setText("Thinking...");

        // 2. ЗАПУСКАЕМ ИИ В ОТДЕЛЬНОМ ПОТОКЕ (SwingWorker)
        // Чтобы интерфейс не завис
        new SwingWorker<String, Void>() {
            @Override
            protected String doInBackground() throws Exception {
                // Реальный запрос к Gemini
                return aiService.getDiagnosis(text);
            }

            @Override
            protected void done() {
                try {
                    String response = get(); // Получаем ответ
                    appendMessage("MediTrack AI", response);
                    
                    // Сохраняем в историю (кратко)
                    new HistoryDAO().saveDiagnosis(username, "AI Consultation: " + text);

                } catch (Exception e) {
                    appendMessage("System", "Error connecting to AI: " + e.getMessage());
                } finally {
                    inputField.setEnabled(true);
                    sendButton.setText("Send");
                    inputField.requestFocus();
                }
            }
        }.execute();
    }
}