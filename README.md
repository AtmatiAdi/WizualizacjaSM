# Charakterystyka tematu projektu WizualizacjaSM
 
Głównym celem projektu jest stworzenie robota Micromouse który będzie posiadał tylko akcelerometr i żyroskop do rozpoznawania położenia i przeszkód. Posiadać on będzie pierścień zamontowany na sprężynie dająca niewielkie możliwości ruchowe, w momencie uderzenia ze ścianą pierścień odchyli się a czujnik to wykryje. Robot jest też projektem na przedmiot Roboty Mobilne. Aby komunikować się z komputerem potrzebne jest urządzenie pośredniczące które będzie również wyposażone w moduł radiowy i interfejs usb aby można było dane przesyłać do komputera. Urządzenie pośredniczące to projekt na Sterowniki Robotów, jest to joystick który posiadać będzie interfejs USB, Wifi, Bluetooth i moduł radiowy, będzie więc idealnym łącznikiem Robot - Komputer.

![GitHub Logo](/images/a.jpg)


# Projekt graficznego interfejsu użytkownika
 
Głównym celem aplikacji jest wyświetlanie informacji uzyskanych z robota, jednak dane te można pogrupować względem istotności i szczegółowości. Projekt zakłada podział interfejsu na oddzielne 3 sekcje, niewykluczone że okaże się potrzebna sekcja czwarta. Każda sekcja odpowiedzialna jest za wyższą warstwę i poziom przetworzenia informacji, rozwiązanie takie ma pewne istotne zalety:

* Stopniowe rozwijanie aplikacji i prostota w sprawdzaniu błędów warstwy wyższej
* Możliwość użycia aplikacji w innych projektach, najniższa warstwa posiada wiele standardowych funkcjonalności
* Prostota w rozbudowie, wystarczy dodać kolejną warstwę
* Łatwość obsługi, wybiera się te warstwy na których się chce pracować, brak natłoku kontrolek itp

![GitHub Logo](/images/app2.PNG)

# Interfejs komunikacyjny

Robot micromouse posiada moduł radiowy NRF24L01 do komunikacji bezprzewodowej. Do połączenia z komputerem wymagane jest urządzenie pośredniczące, joystick posiada tak samo moduł NRF24L01 i wejście USB przez które urządzenie będzie pracować jako Communication Device Class, czyli przez port szeregowy będzie realizowana komunikacja z komputerem. Moduł NRF ma zaimplementowany system CRC, dodatkowo jest opcja aby potwierdzane było odebranie paczek danych poprzez odesłanie danych zwrotnych lub zwykłego sygnału ACK. Dane przesyłane przez USB mają bardzo małe prawdopodobieństwo na błąd i nie ma potrzeby w tym miejscu a ich kontrolę. Moduł NRF pracuje jednocześnie tylko w jednym trybie, odbiornika lub nadajnika. Robot w tym przypadku jest nadajnikiem aby szybko wysyłać dane z czujników do joysticka, potem joystick odsyła te dane do komputera. Aby móc wysyłać dane z komputera i joysticka do robota, zastosowany został tryb potwierdzania odebrania danych z odesłaniem danych zwrotnych zamiast samego potwierdzania sygnałem ACK. Schemat połączeniowy pokazany jest na rysunku: 

![GitHub Logo](/images/interfejsy.png)

# Algorytm Micromouse
Algorytm ma za zadanie wykrywać ściany w labiryncie i zapamiętywać ich ustawienie w pamięci, po odkryciu całego labiryntu wyznaczana będzie ścieżka optymalna z punktu startowego do centrum labiryntu, czyli mety. Następnie robot będzie musiał przebyć wyznaczoną ścieżkę jak najszybciej. Algorytm został zaimplementowany w aplikacji. Takie rozwiązanie bardzo usprawniło tworzenie algorytmu. Istotne jest to że wymiana danych mimo swej prędkości nie jest wstanie zapewnić dokładności w takich operacjach jak przemieszczanie się czy obracanie, liczenie podwójnej całki z tak otrzymywanych danych byłoby bezużyteczne, robot więc posiada funkcje do przemieszczania się i obrotu samodzielnie, zapewnia to dokładność. Jazda o zadane przemieszczenie odbywa się poprzez dostarczenie też informacje o wartości progowej przyspieszenia które oznaczałoby zderzenie się z przeszkodą, niezależnie od przyczyny zakończenia ruchu, robot wysyła wartość przemieszczenia jaką udało mu się wyliczyć na podstawie odczytów z akcelerometru. Podczas obrotu nie podaje się warunku stopu. Uzyskana dokładność obrotu jest bardzo zadowalająca, lecz przemieszczanie się jest obarczone takim błędem że nie możliwe jest zadanie robotowi przemieszczenia większego niż 18 cm (rozmiar jednej komórki labiryntu) bez błędu na tyle dużego który zakłóci działanie algorytmu. Problem ten rozwiązuje sposób przemieszczania się w którym robot zawsze przemieszcza się najdalej jak potrafi, algorytm dostając informację zwrotną dopasowywać ją do labiryntu, jednak takie rozwiązanie powoduje następny problem, robot będzie mógł wpaść w pułapki z których nie będzie dało się w ten sposób wyjechać, rozwiązaniem jest połączenie tych dwóch taktyk tak by algorytm sam decydował o sposobie przemieszczania się.

https://youtu.be/gzY4Sm2h51A - Prezentacja ukończenia projektu

https://www.overleaf.com/read/gbpkgqgbnxns - Raport końcowy
