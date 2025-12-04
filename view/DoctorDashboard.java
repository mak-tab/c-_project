package view;

import dao.HistoryDAO;
import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.util.List;

public class DoctorDashboard extends JFrame {

    public DoctorDashboard(String doctorName) {
        super("MediTrack - Doctor Portal: " + doctorName);
        setSize(600, 400);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        // Заголовок
        JLabel title = new JLabel("Patient Diagnosis History", SwingConstants.CENTER);
        title.setFont(new Font("Arial", Font.BOLD, 18));
        add(title, BorderLayout.NORTH);

        // Таблица
        String[] columns = {"Patient", "Detected Disease", "Time"};
        DefaultTableModel model = new DefaultTableModel(columns, 0);
        JTable table = new JTable(model);
        
        // Загрузка данных
        loadData(model);

        add(new JScrollPane(table), BorderLayout.CENTER);

        // Кнопка выхода/обновления
        JButton refreshBtn = new JButton("Refresh Data");
        refreshBtn.addActionListener(e -> loadData(model));
        add(refreshBtn, BorderLayout.SOUTH);

        setVisible(true);
    }

    private void loadData(DefaultTableModel model) {
        model.setRowCount(0); // Очистка
        List<String[]> records = new HistoryDAO().getAllRecords();
        for (String[] row : records) {
            model.addRow(row);
        }
    }
}