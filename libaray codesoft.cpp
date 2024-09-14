#include <iostream>
#include <vector>
#include <string>
#include <ctime>

class Book {
public:
    std::string title;
    std::string author;
    std::string isbn;
    bool isAvailable;
    std::time_t dueDate;

    Book(std::string t, std::string a, std::string i)
        : title(t), author(a), isbn(i), isAvailable(true) {}

    void checkOut() {
        isAvailable = false;
        std::time(&dueDate);
        // Set due date to 2 weeks from now (14 days * 24 hours * 3600 seconds)
        dueDate += 14 * 24 * 3600;
    }

    void returnBook() {
        isAvailable = true;
        dueDate = 0;
    }

    double calculateFine() const {
        if (isAvailable || dueDate == 0) return 0.0;

        std::time_t now;
        std::time(&now);
        double fine = 0.0;
        double daysLate = std::difftime(now, dueDate) / (24 * 3600);
        if (daysLate > 0) {
            fine = daysLate * 0.5; // Assume $0.5 per day late
        }
        return fine;
    }
};

class BookDatabase {
private:
    std::vector<Book> books;

public:
    void addBook(const Book& book) {
        books.push_back(book);
    }

    Book* findBookByTitle(const std::string& title) {
        for (auto& book : books) {
            if (book.title == title) return &book;
        }
        return nullptr;
    }

    Book* findBookByAuthor(const std::string& author) {
        for (auto& book : books) {
            if (book.author == author) return &book;
        }
        return nullptr;
    }

    Book* findBookByISBN(const std::string& isbn) {
        for (auto& book : books) {
            if (book.isbn == isbn) return &book;
        }
        return nullptr;
    }
};

void displayMenu() {
    std::cout << "1. Add Book\n";
    std::cout << "2. Search Book\n";
    std::cout << "3. Check Out Book\n";
    std::cout << "4. Return Book\n";
    std::cout << "5. Calculate Fine\n";
    std::cout << "6. Exit\n";
    std::cout << "Choose an option: ";
}

void handleUserInput(BookDatabase& db) {
    int choice;
    while (true) {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore(); // Ignore newline character after integer input

        if (choice == 6) break;

        switch (choice) {
            case 1: {
                std::string title, author, isbn;
                std::cout << "Enter book title: ";
                std::getline(std::cin, title);
                std::cout << "Enter book author: ";
                std::getline(std::cin, author);
                std::cout << "Enter book ISBN: ";
                std::getline(std::cin, isbn);
                db.addBook(Book(title, author, isbn));
                std::cout << "Book added.\n";
                break;
            }
            case 2: {
                std::string search;
                std::cout << "Search by title, author, or ISBN? ";
                std::getline(std::cin, search);
                Book* book = nullptr;

                if (search.find_first_of("0123456789") != std::string::npos) {
                    book = db.findBookByISBN(search);
                } else if (search.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != std::string::npos) {
                    book = db.findBookByTitle(search);
                } else {
                    book = db.findBookByAuthor(search);
                }

                if (book) {
                    std::cout << "Title: " << book->title << "\n";
                    std::cout << "Author: " << book->author << "\n";
                    std::cout << "ISBN: " << book->isbn << "\n";
                    std::cout << "Available: " << (book->isAvailable ? "Yes" : "No") << "\n";
                } else {
                    std::cout << "Book not found.\n";
                }
                break;
            }
            case 3: {
                std::string isbn;
                std::cout << "Enter book ISBN to check out: ";
                std::getline(std::cin, isbn);
                Book* book = db.findBookByISBN(isbn);
                if (book && book->isAvailable) {
                    book->checkOut();
                    std::cout << "Book checked out.\n";
                } else {
                    std::cout << "Book not available for checkout.\n";
                }
                break;
            }
            case 4: {
                std::string isbn;
                std::cout << "Enter book ISBN to return: ";
                std::getline(std::cin, isbn);
                Book* book = db.findBookByISBN(isbn);
                if (book && !book->isAvailable) {
                    double fine = book->calculateFine();
                    book->returnBook();
                    std::cout << "Book returned.\n";
                    if (fine > 0) {
                        std::cout << "Fine: $" << fine << "\n";
                    }
                } else {
                    std::cout << "Book was not checked out or does not exist.\n";
                }
                break;
            }
            case 5: {
                std::string isbn;
                std::cout << "Enter book ISBN to calculate fine: ";
                std::getline(std::cin, isbn);
                Book* book = db.findBookByISBN(isbn);
                if (book) {
                    double fine = book->calculateFine();
                    std::cout << "Fine: $" << fine << "\n";
                } else {
                    std::cout << "Book not found.\n";
                }
                break;
            }
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
    }
}

int main() {
    BookDatabase db;
    handleUserInput(db);
    return 0;
}
