#include <iostream>
#include <string>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <windows.h>

using namespace std;

class Song
{
public:

    string title;
    string artist;
    Song* left;
    Song* right;
    Song* parent;

    Song(string t, string a)
    {
        title = t;
        artist = a;
        left = NULL;
        right = NULL;
        parent = NULL;
    }
};

class FavoriteSong
{
public:

    Song* song;
    FavoriteSong* left;
    FavoriteSong* right;
    FavoriteSong* parent;

    FavoriteSong(Song* s)
    {
        song = s;
        left = NULL;
        right = NULL;
        parent = NULL;
    }
};

class FavoritesList
{
private:

    FavoriteSong* root;

    FavoriteSong* insert(FavoriteSong* node, Song* song)
    {
        if (node == NULL)
        {
            return new FavoriteSong(song);
        }

        if (song->title < node->song->title)
        {
            node->left = insert(node->left, song);
            node->left->parent = node;
        }
        else if (song->title > node->song->title)
        {
            node->right = insert(node->right, song);
            node->right->parent = node;
        }
        return node;
    }

    void displayInOrder(FavoriteSong* node)
    {
        if (node != NULL)
        {
            displayInOrder(node->left);
            cout << "\"" << node->song->title << "\" by " << node->song->artist << endl;
            displayInOrder(node->right);
        }
    }

public:

    FavoritesList() 
    {
        root = NULL;
    }

    void addFavorite(Song* song) 
    {
        if (root==NULL) 
        {
            root = new FavoriteSong(song);
            cout << "Added \"" << song->title << "\" to favorites." << endl;
            return;
        }

        FavoriteSong* current = root;
        while (current) 
        {
            if (song->title == current->song->title) 
            {
                cout << "\"" << song->title << "\" is already in favorites." << endl;
                return;
            }
            if (song->title < current->song->title)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }
        root = insert(root, song);
        cout << "Added \"" << song->title << "\" to favorites." << endl;
    }

    void displayFavorites() 
    {
        if (root==NULL) 
        {
            cout << "No favorite songs." << endl;
            return;
        }
        cout << "Favorite Songs:" << endl;
        displayInOrder(root);
    }
};

class Playlist 
{
private:

    Song* root;
    FavoritesList favorites;
    stack<Song*> songHistory;

    Song* insert(Song* node, string title, string artist) 
    {
        if (node == NULL) 
        {
            return new Song(title, artist);
        }

        if (title < node->title) 
        {
            node->left = insert(node->left, title, artist);
            node->left->parent = node;
        }
        else if (title > node->title) 
        {
            node->right = insert(node->right, title, artist);
            node->right->parent = node;
        }
        return node;
    }

    void displayInOrder(Song* node) 
    {
        if (node != NULL) 
        {
            displayInOrder(node->left);
            cout << "\"" << node->title << "\" by " << node->artist << endl;
            displayInOrder(node->right);
        }
    }

    Song* findMin(Song* node) 
    {
        while (node->left != NULL) 
        {
            node = node->left;
        }
        return node;
    }

    Song* deleteNode(Song* root, string title) 
    {
        if (root == NULL)
        {
            return root;
        }

        if (title < root->title)
        {
            root->left = deleteNode(root->left, title);
        }
        else if(title > root->title)
        {
                root->right = deleteNode(root->right, title);
        }
        else 
        {
            if (root->left == NULL) 
            {
                Song* temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == NULL) 
            {
                Song* temp = root->left;
                delete root;
                return temp;
            }

            Song* temp = findMin(root->right);
            root->title = temp->title;
            root->artist = temp->artist;
            root->right = deleteNode(root->right, temp->title);
        }
        return root;
    }

    Song* search(Song* root, string title) 
    {
        if (root == NULL || root->title == title)
        {
            return root;
        }

        if (title < root->title)
        {
            return search(root->left, title);
        }

        return search(root->right, title);
    }

    void shuffleHelper(Song* node, Song** songs, int& index) 
    {
        if (node == NULL)
        {
            return;
        }
        shuffleHelper(node->left, songs, index);
        songs[index++] = node;
        shuffleHelper(node->right, songs, index);
    }

    string toLower(string str) 
    {
        for (char& c : str) 
        {
            c = tolower(c);
        }
        return str;
    }

    void sortSongs(Song* node, Song** songs, int& count) 
    {
        if (node == NULL)
        {
            return;
        }

        sortSongs(node->left, songs, count);
        songs[count++] = node;
        sortSongs(node->right, songs, count);
    }

public:

    string name;

    Playlist(string playlistName) : name(playlistName) 
    {
        root = NULL;
    }

    void addSong(string title, string artist) 
    {
        if (root==NULL) 
        {
            root = new Song(title, artist);
        }
        else 
        {
            root = insert(root, title, artist);
        }
        cout << "Added \"" << title << "\" by " << artist << " to playlist \"" << name << "\"." << endl;
    }

    void removeSong(string title) 
    {
        Song* songToRemove = search(root, title);
        if (!songToRemove) 
        {
            cout << "Song \"" << title << "\" not found." << endl;
            return;
        }

        root = deleteNode(root, title);
        cout << "Removed \"" << title << "\" from playlist \"" << name << "\"." << endl;
    }

    void playSong(string title)
    {
        Song* song = search(root, title);
        if (song)
        {
            songHistory.push(song);
            cout << "Playing \"" << song->title << "\" by " << song->artist << "..." << endl;
        }
        else
        {
            cout << "Song \"" << title << "\" not found in playlist \"" << name << "\"." << endl;
        }
    }

    void addSongToFavorites(string title) 
    {
        Song* song = search(root, title);
        if (song) 
        {
            favorites.addFavorite(song);
        }
        else 
        {
            cout << "Song \"" << title << "\" not found in playlist \"" << name << "\"." << endl;
        }
    }

    void displayFavorites() 
    {
        favorites.displayFavorites();
    }

    void searchSong(string title) 
    {
        Song* song = search(root, title);
        if (song) 
        {
            cout << "Found \"" << song->title << "\" by " << song->artist << " in playlist \"" << name << "\"." << endl;
        }
        else 
        {
            cout << "Song \"" << title << "\" not found in playlist \"" << name << "\"." << endl;
        }
    }

    void displayPlaylist() 
    {
        if (root==NULL) 
        {
            cout << "The playlist \"" << name << "\" is empty." << endl;
            return;
        }
        cout << "Songs in playlist \"" << name << "\":" << endl;
        displayInOrder(root);
    }

    void shufflePlaylist() 
    {
        if (root==NULL) 
        {
            cout << "The playlist \"" << name << "\" is empty." << endl;
            return;
        }
        int count = 0;
        Song* songs[50]; 
        shuffleHelper(root, songs, count);
        srand(time(0));

        for (int i = 0; i < count; ++i) 
        {
            int j = rand() % count;
            swap(songs[i], songs[j]);
        }

        cout << "Shuffled playlist \"" << name << "\":" << endl;

        for (int i = 0; i < count; ++i) 
        {
            cout << "\"" << songs[i]->title << "\" by " << songs[i]->artist << endl;
        }
    }

    void sortPlaylist() 
    {
        if (root==NULL) 
        {
            cout << "The playlist is empty." << endl;
            return;
        }
        Song* songs[50]; 
        int count = 0;

        sortSongs(root, songs, count);

        cout << "Sorted playlist \"" << name << "\":" << endl;

        for (int i = 0; i < count; i++) 
        {
            cout << "\"" << songs[i]->title << "\" by " << songs[i]->artist << endl;
        }
    }

    void showSongHistory() 
    {
        if (songHistory.empty()) 
        {
            cout << "No song history available." << endl;
            return;
        }
        cout << "Song history:" << endl;
        stack<Song*> tempStack = songHistory;

        while (!tempStack.empty()) 
        {
            Song* song = tempStack.top();
            cout << "\"" << song->title << "\" by " << song->artist << endl;
            tempStack.pop();
        }
    }
};

class UserProfile
{
private:

    string username;
    Playlist** playlists; 
    int playlistCount;    
    int capacity;        

    void resizeIfNeeded()
    {
        if (playlistCount >= capacity)
        {
            capacity *= 2;
            Playlist** newPlaylists = new Playlist * [capacity];

            for (int i = 0; i < playlistCount; i++)
            {
                newPlaylists[i] = playlists[i];
            }
            delete[] playlists;
            playlists = newPlaylists;
        }
    }

public:
    UserProfile(string name): username(name), playlistCount(0), capacity(5)
    {
        playlists = new Playlist * [capacity];
    }

    ~UserProfile()
    {
        for (int i = 0; i < playlistCount; i++)
        {
            delete playlists[i];
        }
        delete[] playlists;
    }

    void createPlaylist(string playlistName)
    {
        for (int i = 0; i < playlistCount; i++)
        {
            if (playlists[i]->name == playlistName)
            {
                cout << "Playlist \"" << playlistName << "\" already exists." << endl;
                return;
            }
        }
        resizeIfNeeded();
        playlists[playlistCount++] = new Playlist(playlistName);
        cout << "Created playlist \"" << playlistName << "\"." << endl;
    }

    void deletePlaylist(string playlistName)
    {
        int index = -1;
        for (int i = 0; i < playlistCount; i++)
        {
            if (playlists[i]->name == playlistName)
            {
                index = i;
                break;
            }
        }
        if (index == -1)
        {
            cout << "Playlist \"" << playlistName << "\" not found." << endl;
            return;
        }
        delete playlists[index];

        for (int i = index; i < playlistCount - 1; i++)
        {
            playlists[i] = playlists[i + 1];
        }
        playlistCount--;
        cout << "Deleted playlist \"" << playlistName << "\"." << endl;
    }

    void renamePlaylist(string oldName, string newName)
    {
        int index = -1;
        for (int i = 0; i < playlistCount; i++)
        {
            if (playlists[i]->name == oldName)
            {
                index = i;
                break;
            }
        }
        if (index == -1)
        {
            cout << "Playlist \"" << oldName << "\" not found." << endl;
            return;
        }
        for (int i = 0; i < playlistCount; i++)
        {
            if (playlists[i]->name == newName)
            {
                cout << "Playlist \"" << newName << "\" already exists." << endl;
                return;
            }
        }
        playlists[index]->name = newName;
        cout << "Renamed playlist \"" << oldName << "\" to \"" << newName << "\"." << endl;
    }

    Playlist* selectPlaylist(string playlistName)
    {
        for (int i = 0; i < playlistCount; i++)
        {
            if (playlists[i]->name == playlistName)
            {
                return playlists[i];
            }
        }
        cout << "Playlist \"" << playlistName << "\" not found." << endl;
        return NULL;
    }

    void displayPlaylists()
    {
        if (playlistCount == 0)
        {
            cout << "No playlists available." << endl;
            return;
        }
        cout << "Playlists:" << endl;

        for (int i = 0; i < playlistCount; i++)
        {
            cout << "- " << playlists[i]->name << endl;
        }
    }
};

void loadingAnimation(string message)
{
    cout << message << "..." << endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
}

void delay(int milliseconds)
{
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

void clearScreen()
{
    system("CLS");
}

void displayLogo()
{
    cout << "             #######################################################################################" << endl;
    cout << "             #                                                                                     #" << endl;
    cout << "             #  *       *  *  *****   ******       ***** ******* *****  *****  *****   **     **   #" << endl;
    cout << "             #   *    *    |  |   |   |            |        *    |   |  |      |   |   *  * *  *   #" << endl;
    cout << "             #    *  *     |  *****   ******       *****    *    *****  *****  *****   *   *   *   #" << endl;
    cout << "             #     **      |  |   |   |                |    *    *  *   |      *   *   *       *   #" << endl;
    cout << "             #     **      |  *****   ******       *****    *    *   *  *****  *   *   *       *   # " << endl;
    cout << "             #                                                                                     #" << endl;
    cout << "             #######################################################################################" << endl;
    cout << endl << endl;

    delay(700);
    loadingAnimation("                        Welcome to VIBE STREAM! Your music journey starts here.");
    delay(250);

    loadingAnimation("                             Enjoy exploring and managing your playlists.");
    delay(150);

    loadingAnimation("                                     Let's create some vibes!");
    delay(50);

    cout << "____________________________________________________________________________________________________________________" << endl << endl;
}

void displayMenu() 
{
    cout << "====== Music Player Menu ======" << endl << endl;
    cout << "1. | Create playlist" << endl;
    cout << "2. | Add song to playlist"<<endl;
    cout << "3. | Remove song from playlist" << endl;
    cout << "4. | Display playlist" << endl;
    cout << "5. | Rename playlist" << endl;
    cout << "6. | Shuffle playlist" << endl;
    cout << "7. | Add song to favorites" << endl;
    cout << "8. | View favorites" << endl;
    cout << "9. | Search song in playlist" << endl;
    cout << "10.| Display all playlists" << endl;
    cout << "11.| Sort playlist" << endl;
    cout << "12.| Show song history" << endl;
    cout << "13.| Delete playlist" << endl;
    cout << "14.| Play song" << endl;
    cout << "0. | Exit" << endl << endl;
    cout << "Enter your choice: ";
}

int main() 
{
    system("color CB");

    displayLogo();
    system("pause");

    string username, playlistName, title, artist, oldName, newName;
    int choice;

    cout << "Enter your username: ";
    getline(cin, username);

    loadingAnimation("Creating profile for " + username);
    delay(1000);
    clearScreen();

    loadingAnimation("Welcome to your Vibe Stream account "+ username);
    delay(1500);
    clearScreen();

    UserProfile user(username);

    while (true) 
    {
        displayMenu();
        cin >> choice;
        cin.ignore();

        if (choice == 0) break;

        switch (choice) 
        {
        case 1:
            loadingAnimation("Creating new playlist");

            cout << "Enter playlist name: ";
            getline(cin, playlistName);
            user.createPlaylist(playlistName);

            break;

        case 2:
            loadingAnimation("Adding song to playlist");

            cout << "Enter playlist name: ";
            getline(cin, playlistName);

            if (Playlist* p = user.selectPlaylist(playlistName)) 
            {
                cout << "Enter song title: ";
                getline(cin, title);

                cout << "Enter artist name: ";
                getline(cin, artist);

                p->addSong(title, artist);
            }
            break;

        case 3:
            loadingAnimation("Removing song from playlist");

            cout << "Enter playlist name: ";
            getline(cin, playlistName);

            if (Playlist* p = user.selectPlaylist(playlistName)) 
            {
                cout << "Enter song title to remove: ";
                getline(cin, title);
                p->removeSong(title);
            }
            break;

        case 4:
            loadingAnimation("Displaying playlist");

            cout << "Enter playlist name: ";
            getline(cin, playlistName);

            if (Playlist* p = user.selectPlaylist(playlistName)) 
            {
                p->displayPlaylist();
                system("pause");
            }
            break;

        case 5:
            loadingAnimation("Renaming playlist");

            cout << "Enter current playlist name: ";
            getline(cin, oldName);

            cout << "Enter new playlist name: ";
            getline(cin, newName);

            user.renamePlaylist(oldName, newName);

            break;

        case 6:
            loadingAnimation("Shuffling playlist");

            cout << "Enter playlist name: ";
            getline(cin, playlistName);

            if (Playlist* p = user.selectPlaylist(playlistName)) 
            {
                p->shufflePlaylist();
                system("pause");
            }
            break;

        case 7:
            loadingAnimation("Adding song to favorites");

            cout << "Enter playlist name: ";
            getline(cin, playlistName);

            if (Playlist* p = user.selectPlaylist(playlistName)) 
            {
                cout << "Enter song title to add to favorites: ";
                getline(cin, title);
                p->addSongToFavorites(title);
            }
            break;

        case 8:
            loadingAnimation("Viewing favorites");

            cout << "Enter playlist name to view favorites: ";
            getline(cin, playlistName);

            if (Playlist* p = user.selectPlaylist(playlistName)) 
            {
                p->displayFavorites();
                system("pause");
            }
            break;

        case 9:
            loadingAnimation("Searching for a song");

            cout << "Enter playlist name: ";
            getline(cin, playlistName);

            if (Playlist* p = user.selectPlaylist(playlistName)) 
            {
                cout << "Enter song title to search: ";
                getline(cin, title);
                p->searchSong(title);
                system("pause");
            }
            break;

        case 10:
            loadingAnimation("Displaying all playlists");
            user.displayPlaylists();
            system("pause");

            break;

        case 11:
            loadingAnimation("Sorting playlist");

            cout << "Enter playlist name: ";
            getline(cin, playlistName);

            if (Playlist* p = user.selectPlaylist(playlistName)) 
            {
                p->sortPlaylist();
                system("pause");
            }
            break;

        case 12:
            loadingAnimation("Showing song history");

            cout << "Enter playlist name: ";
            getline(cin, playlistName);

            if (Playlist* p = user.selectPlaylist(playlistName)) 
            {
                p->showSongHistory();
                system("pause");
            }
            break;

        case 13: 
            loadingAnimation("Deleting playlist");

            cout << "Enter playlist name to delete: ";
            getline(cin, playlistName);
            user.deletePlaylist(playlistName);

            break;

        case 14:  
            loadingAnimation("Playing song");

            cout << "Enter playlist name: ";
            getline(cin, playlistName);

            if (Playlist* p = user.selectPlaylist(playlistName))
            {
                cout << "Enter song title to play: ";
                getline(cin, title);
                p->playSong(title);
                system("pause");
            }
            break;

        default:
            loadingAnimation("Invalid choice, please try again");

            break;
        }
        delay(2000);
        clearScreen();
    }
    loadingAnimation("Exiting");

    return 0;
}