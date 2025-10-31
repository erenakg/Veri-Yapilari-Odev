#include <iostream>
#include <string>
using namespace std;


/*
Mehmet Eren Akgül
1306230020
*/



// DayNode yapisi - her gun icin bir node
struct DayNode {
	int day;
	string title = "";
	string content = "";
	string priority = "";
	DayNode* prev;
	DayNode* next;
};

// 30 gunluk circular double linked list olusturur
DayNode* initCalendar(int totalDays = 30) {
	DayNode* head = new DayNode{ 1, "", "", "", nullptr, nullptr };
	DayNode* current = head;

	// 2'den 30'a kadar gunleri olustur
	for (int i = 2; i <= totalDays; i++) {
		DayNode* newNode = new DayNode{ i, "", "", "", current, nullptr };
		current->next = newNode;
		current = newNode;
	}

	// Circular yapmak icin son node'u head'e bagla
	current->next = head;
	head->prev = current;
	return head;
}

// Belirli bir gun numarasina sahip node'u bulur
DayNode* findDay(DayNode* head, int day) {
	DayNode* current = head;
	
	// Circular listede gezinerek istenen gunu bul
	for (int i = 1; i < day; i++) {
		current = current->next;
	}
	
	return current;
}

// Bir gune event ekler
void addEvent(DayNode* head) {
	int day;
	cout << "\nHangi gun icin event eklemek istiyorsunuz? (1-30): ";
	cin >> day;
	
	// Gun numarasi gecerli mi kontrol et
	if (day < 1 || day > 30) {
		cout << "Hatali gun numarasi! Lutfen 1-30 arasi bir deger girin.\n";
		return;
	}
	
	// Ilgili gunu bul
	DayNode* dayNode = findDay(head, day);
	
	// Eger o gun icin zaten bir event varsa
	if (dayNode->title != "") {
		cout << "Bu gun icin zaten bir event var!\n";
		return;
	}
	
	// Event bilgilerini kullanicidan al
	cin.ignore(); // Onceki cin'den kalan newline karakterini temizle
	cout << "Etkinlik basligi: ";
	getline(cin, dayNode->title);
	
	cout << "Etkinlik icerigi: ";
	getline(cin, dayNode->content);
	
	cout << "Oncelik (low/high): ";
	getline(cin, dayNode->priority);
	
	cout << "Event basariyla eklendi!\n";
}

// Bir gundeki event'i duzenler
void editEvent(DayNode* head) {
	int day;
	cout << "\nHangi gunun event'ini duzenlemek istiyorsunuz? (1-30): ";
	cin >> day;
	
	// Gun numarasi gecerli mi kontrol et
	if (day < 1 || day > 30) {
		cout << "Hatali gun numarasi! Lutfen 1-30 arasi bir deger girin.\n";
		return;
	}
	
	// Ilgili gunu bul
	DayNode* dayNode = findDay(head, day);
	
	// Mevcut event bilgilerini goster
	cout << "\n--- Gun " << dayNode->day << " Mevcut Bilgiler ---\n";
	if (dayNode->title != "") {
		cout << "Baslik: " << dayNode->title << endl;
		cout << "Icerik: " << dayNode->content << endl;
		cout << "Oncelik: " << dayNode->priority << endl;
	} else {
		cout << "Bu gun icin event yok.\n";
	}
	
	// Yeni bilgileri al
	cin.ignore(); // Onceki cin'den kalan newline karakterini temizle
	cout << "\nYeni etkinlik basligi: ";
	getline(cin, dayNode->title);
	
	cout << "Yeni etkinlik icerigi: ";
	getline(cin, dayNode->content);
	
	cout << "Yeni oncelik (low/high): ";
	getline(cin, dayNode->priority);
	
	cout << "Event basariyla duzenlendi!\n";
}

// Event'leri listeler ve gezinme imkani saglar
void viewEvents(DayNode* head) {
	int day;
	cout << "\nBaslangic gunu (1-30): ";
	cin >> day;
	
	// Gun numarasi gecerli mi kontrol et
	if (day < 1 || day > 30) {
		cout << "Hatali gun numarasi! Lutfen 1-30 arasi bir deger girin.\n";
		return;
	}
	
	// Ilgili gunu bul
	DayNode* current = findDay(head, day);
	char command;
	
	// Kullanici q'ya basana kadar devam et
	while (true) {
		// Mevcut gunun bilgilerini goster
		cout << "\n========== GUN " << current->day << " ==========\n";
		if (current->title != "") {
			cout << "Baslik: " << current->title << endl;
			cout << "Icerik: " << current->content << endl;
			cout << "Oncelik: " << current->priority << endl;
		} else {
			cout << "Bu gun icin event yok.\n";
		}
		
		cout << "\nKomutlar: [p]revious | [n]ext | [q]uit\n";
		cout << "Seciminiz: ";
		cin >> command;
		
		if (command == 'p' || command == 'P') {
			// Onceki gune git (circular oldugu icin 1'den once 30'a gider)
			current = current->prev;
		} else if (command == 'n' || command == 'N') {
			// Sonraki gune git (circular oldugu icin 30'dan sonra 1'e gider)
			current = current->next;
		} else if (command == 'q' || command == 'Q') {
			// Ana menuye don
			break;
		} else {
			cout << "Gecersiz komut!\n";
		}
	}
}

// Bir gundeki event'i siler
void deleteEvent(DayNode* head) {
	int day;
	cout << "\nHangi gunun event'ini silmek istiyorsunuz? (1-30): ";
	cin >> day;
	
	// Gun numarasi gecerli mi kontrol et
	if (day < 1 || day > 30) {
		cout << "Hatali gun numarasi! Lutfen 1-30 arasi bir deger girin.\n";
		return;
	}
	
	// Ilgili gunu bul
	DayNode* dayNode = findDay(head, day);
	
	// Eger o gun icin event yoksa
	if (dayNode->title == "") {
		cout << "Bu gun icin zaten bir event yok!\n";
		return;
	}
	
	// Event bilgilerini sifirla (node'u silme!)
	dayNode->title = "";
	dayNode->content = "";
	dayNode->priority = "";
	
	cout << "Event basariyla silindi!\n";
}

int main() {
	// 30 gunluk takvim olustur
	DayNode* calendar = initCalendar();
	int choice;

	// Ana menu dongusu
	while (true) {
		cout << "\n===== TAKVIM MENUSU =====\n";
		cout << "1. Event Ekle\n";
		cout << "2. Event Duzenle\n";
		cout << "3. Event Listele\n";
		cout << "4. Event Sil\n";
		cout << "5. Cikis\n";
		cout << "Seciminiz: ";
		cin >> choice;

		switch(choice) {
			case 1: addEvent(calendar); break;
			case 2: editEvent(calendar); break;
			case 3: viewEvents(calendar); break;
			case 4: deleteEvent(calendar); break;
			case 5: 
				cout << "Program sonlandiriliyor...\n"; 
				// Bellek temizligi yapilabilir (opsiyonel)
				return 0;
			default: 
				cout << "Gecersiz secim! Lutfen 1-5 arasi bir deger girin.\n";
		}
	}
	
	return 0;
}
