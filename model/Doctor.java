package model;

public class Doctor extends User {

    public Doctor(int id, String username, String passwordHash) {
        super(id, username, passwordHash, "DOCTOR");
    }

    @Override
    public void openDashboard() {
        new view.DoctorDashboard(getUsername());
    }
}