package view;

import dao.HistoryDAO;
import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.List;

public class DoctorDashboard extends JFrame {

    public DoctorDashboard(String doctorName) {
        super("MediTrack - Doctor Portal: " + doctorName);
        setSize(800, 500); // Чуть шире
        setLocationRelativeTo(null);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        // == ТЕМНАЯ ТЕМА ДЛЯ ДОКТОРА (Чтобы было стильно) ==
        Color darkBg = new Color(40, 44, 52);
        Color textColor = new Color(220, 223, 228);
        getContentPane().setBackground(darkBg);

        // Заголовок
        JLabel title = new JLabel("Patient Interaction History", SwingConstants.CENTER);
        title.setFont(new Font("Segoe UI", Font.BOLD, 20));
        title.setForeground(textColor);
        title.setBorder(BorderFactory.createEmptyBorder(10, 0, 10, 0));
        add(title, BorderLayout.NORTH);

        // Таблица
        String[] columns = {"Patient", "Chat Log (Double click to read)", "Time"};
        DefaultTableModel model = new DefaultTableModel(columns, 0) {
            @Override
            public boolean isCellEditable(int row, int column) {
                return false; // Запрет редактирования
            }
        };
        JTable table = new JTable(model);
        
        // Стилизация таблицы
        table.setRowHeight(30);
        table.setFont(new Font("Segoe UI", Font.PLAIN, 14));
        table.getTableHeader().setFont(new Font("Segoe UI", Font.BOLD, 14));
        
        // Событие: ДВОЙНОЙ КЛИК для чтения полного чата
        table.addMouseListener(new MouseAdapter() {
            public void mousePressed(MouseEvent mouseEvent) {
                JTable table =(JTable) mouseEvent.getSource();
                Point point = mouseEvent.getPoint();
                int row = table.rowAtPoint(point);
                if (mouseEvent.getClickCount() == 2 && row != -1) {
                    // Получаем текст из 2-й колонки (Chat Log)
                    String fullChat = (String) table.getValueAt(row, 1);
                    String patient = (String) table.getValueAt(row, 0);
                    showChatDetails(patient, fullChat);
                }
            }
        });

        loadData(model);

        add(new JScrollPane(table), BorderLayout.CENTER);

        // Кнопка обновления
        JButton refreshBtn = new JButton("Refresh Data");
        refreshBtn.setFont(new Font("Segoe UI", Font.BOLD, 14));
        refreshBtn.setBackground(new Color(97, 175, 239));
        refreshBtn.setForeground(Color.WHITE);
        refreshBtn.addActionListener(e -> loadData(model));
        add(refreshBtn, BorderLayout.SOUTH);

        setVisible(true);
    }

    private void loadData(DefaultTableModel model) {
        model.setRowCount(0); 
        List<String[]> records = new HistoryDAO().getAllRecords();
        for (String[] row : records) {
            model.addRow(row);
        }
    }

    // Всплывающее окно с полным текстом
    private void showChatDetails(String patient, String text) {
        JTextArea area = new JTextArea(text);
        area.setLineWrap(true);
        area.setWrapStyleWord(true);
        area.setFont(new Font("Segoe UI", Font.PLAIN, 16));
        area.setEditable(false);
        
        JScrollPane scroll = new JScrollPane(area);
        scroll.setPreferredSize(new Dimension(500, 300));

        JOptionPane.showMessageDialog(this, scroll, "Chat History: " + patient, JOptionPane.INFORMATION_MESSAGE);
    }
}