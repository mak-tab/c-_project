package service;

import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.nio.charset.StandardCharsets;

public class GeminiService {

    // 🔴 ВСТАВЬ СЮДА СВОЙ КЛЮЧ!
    private static final String API_KEY = "AIzaSyBoykN3NjLS9nWZXHTBIvniy9ib9AsPcIY"; 
    
    // Модель Gemini 1.5 Flash (она быстрая)
    private static final String API_URL = "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=" + API_KEY;

    private final HttpClient httpClient;

    public GeminiService() {
        this.httpClient = HttpClient.newHttpClient();
    }

    public String getDiagnosis(String symptoms) {
        try {
            // Формируем JSON вручную (String manipulation), чтобы не качать библиотеки
            // Мы просим отвечать коротко и четко.
            String prompt = "Act as a medical assistant. The patient says: '" + symptoms + "'. " +
                            "Analyze this. Provide: 1) Possible Condition 2) Recommended OTC Medications 3) Advice. " +
                            "Keep it brief and structured.";

            // Экранируем кавычки в тексте пользователя, чтобы JSON не сломался
            String safePrompt = prompt.replace("\"", "\\\"");

            String jsonBody = "{ \"contents\": [{ \"parts\": [{ \"text\": \"" + safePrompt + "\" }] }] }";

            HttpRequest request = HttpRequest.newBuilder()
                    .uri(URI.create(API_URL))
                    .header("Content-Type", "application/json")
                    .POST(HttpRequest.BodyPublishers.ofString(jsonBody, StandardCharsets.UTF_8))
                    .build();

            HttpResponse<String> response = httpClient.send(request, HttpResponse.BodyHandlers.ofString());

            if (response.statusCode() == 200) {
                return parseGeminiResponse(response.body());
            } else {
                return "Error: Server returned code " + response.statusCode();
            }

        } catch (Exception e) {
            e.printStackTrace();
            return "Connection failed. Please check your internet.";
        }
    }

    // Ручной парсер ответа (Грязный хак, но работает без библиотек)
    private String parseGeminiResponse(String jsonResponse) {
        try {
            // Google отдает структуру: candidates -> content -> parts -> text
            // Мы просто найдем поле "text" и вырежем его содержимое.
            String marker = "\"text\": \"";
            int startIndex = jsonResponse.indexOf(marker);
            
            if (startIndex != -1) {
                startIndex += marker.length();
                int endIndex = jsonResponse.indexOf("\"", startIndex);
                
                // Обработка экранированных переносов строк \n
                String text = jsonResponse.substring(startIndex, endIndex);
                return text.replace("\\n", "\n").replace("\\\"", "\"");
            }
            return "Could not parse AI response.";
        } catch (Exception e) {
            return "Parser error.";
        }
    }
}