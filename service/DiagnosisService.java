package service;

import model.Disease;
import java.util.*;

public class DiagnosisService {

    // База знаний (в идеале грузить из БД, но пока hardcode для скорости, как в твоем C++)
    private List<Disease> knowledgeBase;

    public DiagnosisService() {
        knowledgeBase = new ArrayList<>();
        loadDiseases();
    }

    // Имитация загрузки данных (твои данные из symptoms.txt)
    private void loadDiseases() {
        knowledgeBase.add(new Disease(
            "Common Cold",
            Arrays.asList("sneezing", "runny nose", "sore throat", "mild cough", "fatigue"),
            Arrays.asList("Paracetamol", "Ibuprofen"),
            "A viral infection causing sneezing and sore throat."
        ));
        knowledgeBase.add(new Disease(
            "Influenza",
            Arrays.asList("fever", "chills", "body aches", "headache", "fatigue"),
            Arrays.asList("Oseltamivir", "Paracetamol"),
            "A contagious respiratory illness."
        ));
        // Добавь остальные болезни сюда...
    }

    /**
     * Главный метод анализа.
     * @param userSymptoms список симптомов от пользователя
     * @return Список болезней, отсортированных по вероятности совпадения
     */
    public List<Disease> analyzeSymptoms(List<String> userSymptoms) {
        Map<Disease, Integer> scores = new HashMap<>();

        for (Disease disease : knowledgeBase) {
            int score = 0;
            for (String dSym : disease.getSymptoms()) {
                for (String uSym : userSymptoms) {
                    // Используем Fuzzy Matching (Левенштейн)
                    if (calculateLevenshtein(dSym.toLowerCase(), uSym.toLowerCase()) <= 2) {
                        score++;
                        break; // Симптом засчитан
                    }
                }
            }
            if (score > 0) {
                scores.put(disease, score);
            }
        }

        // Сортировка: у кого больше score, тот первый
        List<Disease> result = new ArrayList<>(scores.keySet());
        result.sort((d1, d2) -> scores.get(d2) - scores.get(d1)); // Descending order

        return result;
    }

    // Твой алгоритм Левенштейна, переписанный на Java
    private int calculateLevenshtein(String x, String y) {
        int[][] dp = new int[x.length() + 1][y.length() + 1];

        for (int i = 0; i <= x.length(); i++) {
            for (int j = 0; j <= y.length(); j++) {
                if (i == 0) {
                    dp[i][j] = j;
                } else if (j == 0) {
                    dp[i][j] = i;
                } else {
                    dp[i][j] = min(
                        dp[i - 1][j - 1] + (x.charAt(i - 1) == y.charAt(j - 1) ? 0 : 1),
                        dp[i - 1][j] + 1,
                        dp[i][j - 1] + 1
                    );
                }
            }
        }
        return dp[x.length()][y.length()];
    }

    private int min(int... numbers) {
        return Arrays.stream(numbers).min().orElse(Integer.MAX_VALUE);
    }
}