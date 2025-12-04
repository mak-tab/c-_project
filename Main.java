import view.LoginView;
import javax.swing.SwingUtilities;

public class Main {
    public static void main(String[] args) {
        // Запуск Swing в потоке обработки событий (Academic Standard)
        SwingUtilities.invokeLater(() -> {
            try {
                // Пытаемся установить системный стиль окон (чтобы выглядело как Windows/Mac, а не как старая Java)
                javax.swing.UIManager.setLookAndFeel(javax.swing.UIManager.getSystemLookAndFeelClassName());
            } catch (Exception e) {
                e.printStackTrace();
            }
            
            // Запуск окна логина
            new LoginView();
        });
    }
}