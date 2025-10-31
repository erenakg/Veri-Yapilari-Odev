#include <iostream>
#include <string>
using namespace std;

/*
Mehmet Eren Akgül
1306230020
*/

struct Appointment {
    string patientName;
    double time;          // örn: 9.30 (saat 09:30)
    int duration;         // dakika cinsinden
    Appointment* next;
};

// Dakikayı saat formatına çeviren yardımcı fonksiyon - YUKARIYA TAŞINDI
double addMinutesToTime(double time, int minutes) {
    int hour = (int)time;
    int currentMinute = (int)((time - hour) * 100);
    
    int totalMinutes = currentMinute + minutes;
    int newHour = hour + (totalMinutes / 60);
    int newMinute = totalMinutes % 60;
    
    return newHour + (newMinute / 100.0);
}

// Saati düzgün formatta yazdıran fonksiyon
void printTime(double time) {
    int hour = (int)time;
    int minute = (int)((time - hour) * 100 + 0.5); // 0.30 * 100 = 30 dakika
    
    cout << hour << ".";
    if (minute < 10) cout << "0";
    cout << minute;
}

// Tüm randevuları sırayla yazdırır
void printAll(Appointment* head) {
    if (head == nullptr) {
        cout << "Henuz hic randevu bulunmuyor." << endl;
        return;
    }
    
    cout << endl << "Tum Randevular:" << endl;
    cout << "Hasta Adi - Saat - Sure (dk)" << endl;
    
    Appointment* current = head;
    while (current != nullptr) {
        cout << current->patientName << " - ";
        printTime(current->time);
        cout << " - " << current->duration << " dk" << endl;
        current = current->next;
    }
}

// Verilen saatten sonraki randevuları listeler
void printFromTime(Appointment* head, double time) {
    if (head == nullptr) {
        cout << "Henuz hic randevu bulunmuyor." << endl;
        return;
    }
    
    cout << endl;
    printTime(time);
    cout << " saatinden sonraki randevular:" << endl;
    
    Appointment* current = head;
    bool found = false;
    while (current != nullptr) {
        if (current->time >= time) {
            cout << current->patientName << " - ";
            printTime(current->time);
            cout << " - " << current->duration << " dk" << endl;
            found = true;
        }
        current = current->next;
    }
    
    if (!found) {
        cout << "Bu saatten sonra randevu bulunmuyor." << endl;
    }
}

// Yeni randevuyu saat sırasına göre listeye ekler
void insertAppointment(Appointment*& head, string name, double time, int duration) {
    // Süre kontrolü
    if (duration <= 0) {
        cout << "Hata: Randevu suresi 0'dan buyuk olmali!" << endl;
        return;
    }
    
    // Saat formatı kontrolü
    int hour = (int)time;
    int minute = (int)((time - hour) * 100);
    
    if (minute < 0 || minute > 59) {
        cout << "Hata: Gecersiz saat formati! Dakika 00-59 arasinda olmali." << endl;
        cout << "Ornek: 14.30 (dogru), 14.61 (yanlis)" << endl;
        return;
    }
    
    // Saat aralığı kontrolü
    if (hour < 9 || hour > 17) {
        cout << "Hata: Randevu saati 09.00 - 17.00 arasinda olmali!" << endl;
        return;
    }
    
    // Negatif saat kontrolü
    if (time < 0) {
        cout << "Hata: Saat negatif olamaz!" << endl;
        return;
    }
    
    double endTime = addMinutesToTime(time, duration);
    if (endTime > 17.00) {
        cout << "Hata: Randevu bitis saati 17.00'yi gecemez!" << endl;
        return;
    }
    
    // Çakışma kontrolü
    Appointment* current = head;
    while (current != nullptr) {
        double currentEndTime = addMinutesToTime(current->time, current->duration);
        
        // Yeni randevu mevcut randevuyla çakışıyor mu?
        if ((time >= current->time && time < currentEndTime) || 
            (endTime > current->time && endTime <= currentEndTime) ||
            (time <= current->time && endTime >= currentEndTime)) {
            cout << "Hata: Bu saat araliginda zaten bir randevu var!" << endl;
            cout << "Cakisan randevu: " << current->patientName << " - ";
            printTime(current->time);
            cout << " (" << current->duration << " dk)" << endl;
            return;
        }
        current = current->next;
    }
    
    // Yeni node oluştur
    Appointment* newAppointment = new Appointment();
    newAppointment->patientName = name;
    newAppointment->time = time;
    newAppointment->duration = duration;
    newAppointment->next = nullptr;
    
    // Liste boşsa veya yeni randevu en başa eklenecekse
    if (head == nullptr || head->time > time) {
        newAppointment->next = head;
        head = newAppointment;
    } else {
        // Doğru pozisyonu bul ve ekle
        Appointment* prev = head;
        while (prev->next != nullptr && prev->next->time < time) {
            prev = prev->next;
        }
        newAppointment->next = prev->next;
        prev->next = newAppointment;
    }
    
    cout << "Randevu basariyla eklendi!" << endl;
}

// Verilen saate denk gelen randevuyu siler
void deleteAppointment(Appointment*& head, double time) {
    if (head == nullptr) {
        cout << "Silinecek randevu bulunamadi." << endl;
        return;
    }
    
    // İlk node silinecekse
    if (head->time == time) {
        Appointment* temp = head;
        head = head->next;
        cout << "Randevu silindi: " << temp->patientName << " - ";
        printTime(temp->time);
        cout << endl;
        delete temp;
        return;
    }
    
    // Diğer node'ları kontrol et
    Appointment* current = head;
    while (current->next != nullptr) {
        if (current->next->time == time) {
            Appointment* temp = current->next;
            current->next = temp->next;
            cout << "Randevu silindi: " << temp->patientName << " - ";
            printTime(temp->time);
            cout << endl;
            delete temp;
            return;
        }
        current = current->next;
    }
    
    cout << "Bu saatte randevu bulunamadi." << endl;
}

// Gün boyunca dolu olunan toplam süreyi hesaplar
int totalBusyMinutes(Appointment* head) {
    int total = 0;
    Appointment* current = head;
    
    while (current != nullptr) {
        total += current->duration;
        current = current->next;
    }
    
    return total;
}

// Belirli bir saatten sonra ilk boş zamanı önerir
double suggestNextAvailable(Appointment* head, double startTime) {
    if (startTime > 17.00) {
        return -1; // Çalışma saatleri dışında
    }
    
    if (head == nullptr) {
        return startTime; // Liste boşsa, istenen saatten itibaren uygun
    }
    
    double currentTime = startTime;
    Appointment* current = head;
    
    while (current != nullptr) {
        // Eğer istenen saat mevcut randevudan önce ise
        if (currentTime < current->time) {
            return currentTime; // Bu aralık boş
        }
        
        // Mevcut randevunun bitiş saatini DOĞRU hesapla
        double endTime = addMinutesToTime(current->time, current->duration);
        
        // Eğer istenen saat mevcut randevuyla çakışıyorsa
        if (currentTime < endTime) {
            currentTime = endTime; // Randevunun bitiminden sonra öner
        }
        
        current = current->next;
    }
    
    // Son randevudan sonra uygun saat varsa
    if (currentTime < 17.00) {
        return currentTime;
    }
    
    return -1; // Uygun saat yok
}

// Menüyü göster
void showMenu() {
    cout << endl << "========== RANDEVU PLANLAMA ==========" << endl;
    cout << "1. Tum randevulari listele" << endl;
    cout << "2. Belirli bir saatten sonraki randevulari goster" << endl;
    cout << "3. Yeni randevu ekle" << endl;
    cout << "4. Randevu sil" << endl;
    cout << "5. Toplam dolu sureyi hesapla" << endl;
    cout << "6. Ilk uygun zamani oner" << endl;
    cout << "0. Cikis" << endl;
    cout << "Seciminiz: ";
}

// Belleği temizle
void clearMemory(Appointment* head) {
    while (head != nullptr) {
        Appointment* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    Appointment* head = nullptr;
    int choice;
    
    cout << "Poliklinik Randevu Planlama Sistemi" << endl;
    cout << "Calisma saatleri: 09.00 - 17.00" << endl;
    
    do {
        showMenu();
        cin >> choice;
        
        switch (choice) {
            case 1: {
                printAll(head);
                break;
            }
            case 2: {
                double time;
                cout << "Baslangic saatini girin (orn: 14.30): ";
                cin >> time;
                printFromTime(head, time);
                break;
            }
            case 3: {
                string name;
                double time;
                int duration;
                
                cout << "Hasta adini girin: ";
                cin.ignore();
                getline(cin, name);
                cout << "Randevu saatini girin (orn: 14.30): ";
                cin >> time;
                cout << "Islem suresini girin (dakika): ";
                cin >> duration;
                
                insertAppointment(head, name, time, duration);
                break;
            }
            case 4: {
                double time;
                cout << "Silinecek randevu saatini girin: ";
                cin >> time;
                deleteAppointment(head, time);
                break;
            }
            case 5: {
                int total = totalBusyMinutes(head);
                cout << "Toplam dolu sure: " << total << " dakika (" 
                     << total / 60 << " saat " << total % 60 << " dakika)" << endl;
                break;
            }
            case 6: {
                double startTime;
                cout << "Baslangic saatini girin (orn: 14.30): ";
                cin >> startTime;
                
                double nextAvailable = suggestNextAvailable(head, startTime);
                if (nextAvailable == -1) {
                    cout << "Bu saatten sonra uygun zaman bulunamadi." << endl;
                } else {
                    cout << "Ilk uygun saat: ";
                    printTime(nextAvailable);
                    cout << endl;
                }
                break;
            }
            case 0: {
                cout << "Program sonlandiriliyor..." << endl;
                break;
            }
            default: {
                cout << "Gecersiz secim! Lutfen tekrar deneyin." << endl;
                break;
            }
        }
        
    } while (choice != 0);
    
    // Belleği temizle
    clearMemory(head);
    
    return 0;
}