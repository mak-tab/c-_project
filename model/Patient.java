package model;

public class Patient extends User {

    public Patient(int id, String username, String passwordHash) {
        // Вызываем конструктор родителя (User)
        // Жестко задаем роль "PATIENT"
        super(id, username, passwordHash, "PATIENT");
    }

    @Override
    public void openDashboard() {
        new view.PatientDashboard(getUsername());
    }
}
