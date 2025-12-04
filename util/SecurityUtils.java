package util;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * Утилитарный класс для безопасности.
 * Отвечает за хэширование паролей.
 */
public class SecurityUtils {

    // Приватный конструктор, чтобы нельзя было создать экземпляр утилитного класса
    private SecurityUtils() {}

    /**
     * Преобразует обычный пароль в SHA-256 хэш.
     * * @param originalPassword "Сырой" пароль (например, "1234")
     * @return Хэшированная строка (например, "a665a45920422f9d417e4867efdc4fb...")
     */
    public static String hashPassword(String originalPassword) {
        try {
            // Используем встроенный в Java алгоритм SHA-256
            MessageDigest digest = MessageDigest.getInstance("SHA-256");
            
            // Получаем массив байт
            byte[] encodedhash = digest.digest(originalPassword.getBytes());
            
            // Преобразуем байты в читаемую Hex-строку (16-ричную)
            return bytesToHex(encodedhash);
        } catch (NoSuchAlgorithmException e) {
            throw new RuntimeException("Error initializing SHA-256", e);
        }
    }

    // Вспомогательный метод: байты -> строка
    private static String bytesToHex(byte[] hash) {
        StringBuilder hexString = new StringBuilder(2 * hash.length);
        for (byte b : hash) {
            String hex = Integer.toHexString(0xff & b);
            if (hex.length() == 1) {
                hexString.append('0');
            }
            hexString.append(hex);
        }
        return hexString.toString();
    }
}