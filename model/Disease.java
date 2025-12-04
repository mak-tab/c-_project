package model;

import java.util.List;

public class Disease {
    private String name;
    private List<String> symptoms;
    private List<String> medications;
    private String description;

    public Disease(String name, List<String> symptoms, List<String> medications, String description) {
        this.name = name;
        this.symptoms = symptoms;
        this.medications = medications;
        this.description = description;
    }

    // Getters
    public String getName() { return name; }
    public List<String> getSymptoms() { return symptoms; }
    public List<String> getMedications() { return medications; }
    public String getDescription() { return description; }
}