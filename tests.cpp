#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <string>
#include "task.cpp"
using namespace std;

void createTestFile(const string& name, const string& content) {
    ofstream file(name);
    file << content;
    file.close();
}

TEST(task, Test_for_file0) {
    stringstream output;
    streambuf *old_cout = cout.rdbuf(output.rdbuf());

    stringstream error_stream;
    streambuf *old_cerr = cerr.rdbuf(error_stream.rdbuf());

    const string file0 = "..\\file0.txt";
    const string out =
            "-3\n"
            "09:00 19:00\n"
            "10\n"
            "09:00 1 client1\n"
            "10:00 2 client1 1\n"
            "12:00 4 client1\n";

    createTestFile(file0, out);

    int result = run_task(file0);

    cout.rdbuf(old_cout);
    cerr.rdbuf(old_cerr);

    string correct = "Error in 1 string: wrong count of tables\n";
    ASSERT_EQ(result, 1);
    ASSERT_EQ(error_stream.str(), correct);
}

TEST(task, Test_for_file1) {
    stringstream output;
    streambuf *old_cout = cout.rdbuf(output.rdbuf());

    stringstream error_stream;
    streambuf *old_cerr = cerr.rdbuf(error_stream.rdbuf());

    const string file1 = "..\\file1.txt";
    const string out =
            "3\n"
            "9:00 19:00\n"
            "10\n"
            "09:00 1 client1\n"
            "10:00 2 client1 1\n"
            "12:00 4 client1\n";

    createTestFile(file1, out);

    int result = run_task(file1);

    cout.rdbuf(old_cout);
    cerr.rdbuf(old_cerr);

    string correct = "Error in 2 string: wrong time\n";
    ASSERT_EQ(result, 1);
    ASSERT_EQ(error_stream.str(), correct);
}

TEST(task, Test_for_file2) {
    stringstream output;
    streambuf *old_cout = cout.rdbuf(output.rdbuf());

    stringstream error_stream;
    streambuf *old_cerr = cerr.rdbuf(error_stream.rdbuf());

    const string file2 = "..\\file2.txt";
    const string out =
            "3\n"
            "09:00 19:70\n"
            "10\n"
            "09:00 1 client1\n"
            "10:00 2 client1 1\n"
            "12:00 4 client1\n";

    createTestFile(file2, out);

    int result = run_task(file2);

    cout.rdbuf(old_cout);
    cerr.rdbuf(old_cerr);

    string correct = "Error in 2 string: wrong time\n";
    ASSERT_EQ(result, 1);
    ASSERT_EQ(error_stream.str(), correct);
}

TEST(task, Test_for_file3) {
    stringstream output;
    streambuf *old_cout = cout.rdbuf(output.rdbuf());

    stringstream error_stream;
    streambuf *old_cerr = cerr.rdbuf(error_stream.rdbuf());

    const string file3 = "..\\file3.txt";
    const string out =
            "3\n"
            "09:00 19:00\n"
            "10.5\n"
            "09:00 1 client1\n"
            "10:00 2 client1 1\n"
            "12:00 4 client1\n";

    createTestFile(file3, out);

    int result = run_task(file3);

    cout.rdbuf(old_cout);
    cerr.rdbuf(old_cerr);

    string correct = "Error in 2 string: wrong time\n";
    ASSERT_EQ(result, 1);
    ASSERT_EQ(error_stream.str(), correct);
}

TEST(task, Test_for_file4) {
    stringstream output;
    streambuf *old_cout = cout.rdbuf(output.rdbuf());

    stringstream error_stream;
    streambuf *old_cerr = cerr.rdbuf(error_stream.rdbuf());

    const string file4 = "..\\file4.txt";
    const string out =
            "3\n"
            "09:00 19:00\n"
            "10\n"
            "09:00 1 client1\n"
            "08:00 2 client1 1\n"
            "12:00 4 client1\n";

    createTestFile(file4, out);

    int result = run_task(file4);

    cout.rdbuf(old_cout);
    cerr.rdbuf(old_cerr);

    string correct = "Error in 5 string: wrong time\n";
    ASSERT_EQ(result, 1);
    ASSERT_EQ(error_stream.str(), correct);
}

TEST(task, Test_for_file5_correct) {
    stringstream output;
    streambuf *old_cout = cout.rdbuf(output.rdbuf());

    stringstream error_stream;
    streambuf *old_cerr = cerr.rdbuf(error_stream.rdbuf());

    const string file5 = "..\\file5.txt";
    const string out =
            "3\n"
            "09:00 19:70\n"
            "10\n"
            "09:00 1 client1\n"
            "10:00 2 client1 1\n"
            "12:00 4 client1\n";

    createTestFile(file5, out);

    int result = run_task(file5);

    cout.rdbuf(old_cout);
    cerr.rdbuf(old_cerr);

    string correct = 
    "09:00\n09:00 1 client1\n10:00 2 client1 1\n2:00 4 client1\n19:00\n1 20 02:00\n2 0 00:00\n3 0 00:00\n";
    ASSERT_EQ(result, 0);
    ASSERT_EQ(output.str(), correct);
}

TEST(task, Test_for_TEST_FILE_correct) {
    stringstream output;
    streambuf *old_cout = cout.rdbuf(output.rdbuf());

    stringstream error_stream;
    streambuf *old_cerr = cerr.rdbuf(error_stream.rdbuf());

    const string file = "..\\file.txt";
    const string out =
            "33\n"
            "09:00 19:00\n"
            "10\n"
            "08:48 1 client1\n"
            "09:41 1 client1\n"
            "09:48 1 client2\n"
            "09:52 3 client1\n"
            "09:54 2 client1 1\n"
            "10:25 2 client2 2\n"
            "10:58 1 client3\n"
            "10:59 2 client3 3\n"
            "11:30 1 client4\n"
            "11:35 2 client4 2\n"
            "11:45 3 client4\n"
            "12:33 4 client1\n"
            "12:43 4 client2\n"
            "15:52 4 client4\n";


    createTestFile(file, out);

    int result = run_task(file);

    cout.rdbuf(old_cout);
    cerr.rdbuf(old_cerr);

    string correct = 
        "09:00\n08:48 1 client1\n08:48 13 NotOpenYet\n09:41 1 client1\n09:48 1 client2\n09:52 3 client1\n09:52 13 ICanWaitNoLonger!\n09:54 2 client1 1\n10:25 2 client2 2\n10:58 1 client3\n10:59 2 client3 3\n11:30 1 client4\n11:35 2 client4 2\n11:35 13 PlaceIsBusy\n11:45 3 client4\n12:33 4 client1\n12:33 12 client4 1\n12:43 4 client2\n15:52 4 client4\n19:00 11 client3\n19:00\n1 70 05:58\n2 30 02:18\n3 90 08:01";

    ASSERT_EQ(result, 0);
    ASSERT_EQ(output.str(), correct);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
