# Patient-Record-Management
# 🏥 Hospital Record Management System in C

This project is a simple command-line based Hospital Record Management System developed in C. It allows users to manage patient records and assign doctors based on symptoms, using file handling and basic struct-based data management.

## 📂 Features

- Add a new patient with ID, name, age, symptoms, and appointment date
- View patient history by ID or name
- Assign doctors based on patient symptoms (`doctor.txt`)
- Update patient records
- Delete patient records
- Uses persistent storage through `patient.txt`

## 🛠️ File Structure

- `main.c` – Contains the full implementation
- `patient.txt` – Stores all patient records (created automatically if not present)
- `doctor.txt` – Doctor database used for assigning based on symptoms

### Sample doctor.txt format:

```text
101:Dr. Sharma:General:Physician:1234567890:cough:1,2,3
102:Dr. Mehta:Cardiology:Cardiologist:9876543210:chest pain:4,5
🧾 Notes
Maximum 5 symptoms per patient

Doctor assignment is based on exact match of one symptom from doctor.txt

For unmatched symptoms, the default assigned doctor is General Physician
👨‍💻 Author
Made with ❤️ by [Your Name]
B.Tech CSE @ PES University
Also pursuing BS in Data Science @ IITM Online


