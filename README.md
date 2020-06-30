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


# Linki

https://youtu.be/gzY4Sm2h51A - Prezentacja ukończenia projektu

https://www.overleaf.com/read/gbpkgqgbnxns - Raport końcowy
