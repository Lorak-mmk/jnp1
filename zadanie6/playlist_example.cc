#include <iostream>
#include "lib_playlist.h"

#define PlayablePtr std::shared_ptr<IPlayable>
#define PlaylistPtr std::shared_ptr<Playlist>

int test1() {
    Player player;

    auto mishmash = player.createPlaylist("mishmash");
    auto armstrong = player.createPlaylist("armstrong");
    auto whatAWonderfulWorld = player.openFile(File("audio|artist:Louis Armstrong|title:What a Wonderful World|"
                                                    "I see trees of green, red roses too..."));
    auto helloDolly = player.openFile(File("audio|artist:Louis Armstrong|title:Hello, Dolly!|"
                                           "Hello, Dolly! This is Louis, Dolly"));
    armstrong->add(whatAWonderfulWorld);
    armstrong->add(helloDolly);
    auto direstraits = player.openFile(File("audio|artist:Dire Straits|title:Money for Nothing|"
                                            "Now look at them yo-yo's that's the way you do it..."));
    auto cabaret = player.openFile(File("video|title:Cabaret|year:1972|Qvfcynlvat Pnonerg"));


    mishmash->add(cabaret);
    mishmash->add(armstrong);
    mishmash->add(direstraits, 1);
    mishmash->add(direstraits);

    std::cout << "=== Playing 'mishmash' (default sequence mode)" << std::endl;
    mishmash->play();

    std::cout << "=== Playing 'mishmash' (shuffle mode, seed 0 for std::default_random_engine)" << std::endl;
    mishmash->setMode(createShuffleMode(0));
    mishmash->play();

    std::cout << "=== Playing 'mishmash' (removed cabaret and last direstraits, odd-even mode)" << std::endl;
    mishmash->remove(0);
    mishmash->remove();
    mishmash->setMode(createOddEvenMode());
    mishmash->play();

    std::cout << "=== Playing 'mishmash' (sequence mode, 'armstrong' odd-even mode)" << std::endl;
    armstrong->setMode(createOddEvenMode());
    mishmash->setMode(createSequenceMode());
    mishmash->play();

    try {
        auto unsupported = player.openFile(File("mp3|artist:Unsupported|title:Unsupported|Content"));
    } catch (PlayerException const& e) {
        std::cout << e.what() << std::endl;
    }

    try {
        auto corrupted = player.openFile(File("Corrupt"));
    } catch (PlayerException const& e) {
        std::cout << e.what() << std::endl;
    }

    try {
        auto corrupted = player.openFile(File("audio|artist:Louis Armstrong|title:Hello, Dolly!|%#!@*&"));
    } catch (PlayerException const& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}

int test2() {
    Player player;

    auto p1 = player.createPlaylist("ta sama nazwa");
    auto p2 = player.createPlaylist("ta sama nazwa");
    auto audioFile = File("audio|artist:a|title:t|u:|tresc");
    auto audio = player.openFile(audioFile);

    p1->add(audio);
    p2->add(audio);
    std::cout << "p1 play" << std::endl;
    p1->play();
    std::cout << "p2 play" << std::endl;
    p2->play();
    p1->add(p2);
    std::cout << "p2 play" << std::endl;
    p2->play();
    std::cout << "p1 play" << std::endl;
    p1->play();

    try {
        p2->add(p1);
    } catch (PlayerException const& e) {
        std::cout << e.what() << std::endl;
    }

    auto q1 = player.createPlaylist("A");
    auto q2 = player.createPlaylist("B");
    auto q3 = player.createPlaylist("C");
    auto q4 = player.createPlaylist("D");
    auto q5 = player.createPlaylist("E");
    auto q6 = player.createPlaylist("F");
    auto q7 = player.createPlaylist("G");

    q1->add(q2);
    q1->add(q2);
    q2->add(q3);
    q2->add(q3);
    q3->add(q4);
    q3->add(q4);
    q4->add(q5);
    q4->add(q5);
    q5->add(q6);
    q5->add(q6);
    q6->add(q7);
    q6->add(q7);

    try {
        q6->add(q1);
    } catch (PlayerException const& e) {
        std::cout << e.what() << std::endl;
    }

    q1->play();

    auto r1 = player.createPlaylist("sama w sobie");
    try {
        r1->add(r1);
    } catch (PlayerException const& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}

int test3() {
    Player player;

    PlayablePtr audio;
    PlaylistPtr playlist;

    {
        std::string_view buffer_v, buffer2_v;

        std::string buffer("audio|artist:a|title:t|content");
        buffer_v = std::string_view(buffer);

        audio = player.openFile(File(buffer));
        audio->play();
        buffer[13] = 'b';
        audio->play();

        std::string buffer2("super nazwa");
        buffer2_v = std::string_view(buffer2);

        playlist = player.createPlaylist(buffer2);
        buffer2[0] = 'd';
        playlist->add(audio);
        buffer[13] = 'c';
        playlist->add(audio);
        playlist->play();

        std::cout << buffer_v << std::endl;
        std::cout << buffer2_v << std::endl;
    }

    playlist->play();


    File file("u|u");
    {
        std::string buffer("audio|artist:kuki|title:cieple dranie|Skurwysyny!");
        file = File(buffer);
    }

    audio = player.openFile(file);
    audio->play();

    return 0;
}

int test4() {
    Player player;

    auto p1 = player.createPlaylist("lmao");
    auto a1 = player.openFile(File("audio|artist:a|title:t|c"));

    //indeksy
    try { p1->add(a1, 1); return 1; } catch(PlayerException const& e) {}
    p1->add(a1);
    try { p1->add(a1, 2); return 1; } catch(PlayerException const& e) {}
    try { p1->remove(1); return 1; } catch(PlayerException const& e) {}
    p1->remove(0);

    //tresc i metadane
    File f1("audio|artist:a|title:t|"); //dobra
    File f2("audio|artist:a|title:t|.,.,'' ' .?? ???!!?"); //dobra
    File f3("audio|artist:a|title:t|<>"); //zla
    File f4("audio|artist:a|title:t|#include lmao"); //zla

    try { File f5("audio|t|tresc"); return 1; } catch(PlayerException const& e) {}

    player.openFile(f1);
    player.openFile(f2);
    try { player.openFile(f3); return 1; } catch(PlayerException const& e) {}
    try { player.openFile(f4); return 1; } catch(PlayerException const& e) {}

    File f5("audio|artist:czy:dziala:dobrze:dwukropek?|title:sprawdzmy :)|");
    File f6("video|title:to nie powinno sie wypisac!|title:czy dziala przeslanianie?|year:10|");
    File f7("video|title:2012|year:2020|svyz b glz wnx fjvng cvreqbyany antenal 8 yng cb glz wnx fjvng cvreqbyany 1234,.!?");
    File f8("video|year:czy niepoprawny rok po przeslonienu wywali program?|year:2019|title:2019|abcdefGHIJKLM");
    File f9("video|title:brakuje roku|");
    File f10("audio|artist:brakuje tytulu|");
    File f11("audio|artist:git|title:git|metadane?:wlasnie ze nie xD");
    File f12("audio|artist:brakuje tytulu czy kontentu?|title:to nie tytul, to kontent");

    auto p2 = player.createPlaylist("metatresc");

    p2->add(player.openFile(f5));
    p2->add(player.openFile(f6));
    p2->add(player.openFile(f7));
    p2->add(player.openFile(f8));

    try { player.openFile(f9); return 1; } catch(PlayerException const&e) { std::cout << e.what() << std::endl; };
    try { player.openFile(f10); return 1; } catch(PlayerException const&e) { std::cout << e.what() << std::endl; };

    p2->add(player.openFile(f11));

    try { player.openFile(f12); return 1; } catch(PlayerException const&e) { std::cout << e.what() << std::endl; };

    p2->play();

    return 0;
}



int rng() {
    static long long a = 1234;
    static long long b = 5678;
    static long long c = 1234567897;

    a = (a * a + b) % c;
    b = (b * b + 1) % c;

    return a;
}

std::string rsg(size_t len) {
    std::string out = "";
    for (int i = 0; i < len; i++)
        out += (char)(rng() % 26 + 'a');
    return out;
}

int test6() {
    int N = 100;
    Player player;
    std::vector<PlaylistPtr> p;
    std::vector<PlayablePtr> q;

    for (int i = 0; i < N; i++) {
        p.push_back(player.createPlaylist(rsg(15)));
        int r = rng()%3;
        if (r == 1) p.back()->setMode(createShuffleMode(rng()));
        if (r == 2) p.back()->setMode(createOddEvenMode());
    }

    for (int i = 0; i < N; i++) {
        File file("audio|artist:" + rsg(15) + "|title:" + rsg(15) + "|" + rsg(20));
        q.push_back(player.openFile(file));
    }

    std::vector<int> lens(N);

    for (int i = 0; i < N; i++) {
        lens[i] = rng() % 20;
        for (int j = 0; j < lens[i]; j++) {
            p[i]->add(q[rng() % N]);
        }
        if (i > 0) {
            int par = rng() % i;
            lens[par]++;
            p[par]->add(p[i], rng() % lens[par]);
        }
    }

    p[0]->play();
    return 0;
}


int main() {
    std::cout << "\n\nTEST 1 - przykladowy\n";
    test1();

    std::cout << "\n\nTEST 2 - zagniezdzanie playlist + sprawdzanie cykli\n";
    test2();

    std::cout << "\n\nTEST 3 - prawidlowe trzymanie napisow\n";
    test3();

    std::cout << "\n\nTEST 4 - watpliwosci z tresci zadania\n";
    if (test4() != 0) {
        std::cout << "TEST 4 jebl\n";
        return 1;
    };

    std::cout << "\n\nTEST 6 - duze losowe drzewo\n";
    test6();
}
