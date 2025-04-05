json@[] nullJsonArray() {
    return { };
}

json@ nullJsonObject() {
    return json();
}

class TestChild {
    TestChild() {
    }
    TestChild(const json& j) {
        m_child = bool(j["child"]);
    }
    json opImplConv() {
        json@ j = json();
        j["child"] = m_child;
        return j;
    }

    void setChild(bool child) {
        m_child = child;
    }
    bool m_child = false;
};

class TestJSON {
    TestJSON() {
    }
    TestJSON(const json& j) {
        m_child = j["child"];
        m_test = double(j["test"]);
        m_isOpen = bool(j["isOpen"]);
        json testArray = j["testArray"];
        for (uint i = 0; i < testArray.getSize(); ++i) {
            m_testArray.insertAt(i, testArray[i]);
        }
    }

    json opImplConv() {
        json@ j = json();

        j["child"] = m_child;
        j["test2"] = nullJsonObject();
        j["test"] = m_test;
        j["isOpen"] = m_isOpen;
        j["testArray"] = nullJsonArray();
        foreach (TestChild ch : m_testArray) {
            j["testArray"].push_back(ch);
        }

        return j;
    }

    private TestChild m_child;
    private double m_test = 1.0;
    private bool m_isOpen = false;
    array<TestChild> m_testArray;
}

void initialize() {
    print(LogLevel::Info, "Initialize");
    TestJSON test = jsonParseFile("test.json");
    json@ js = test;
    print(LogLevel::Info, "\n" + js.dump(4));
    jsonWriteFile(js, "test.json");
}

bool didDraw = false;
void draw() {
    if (!didDraw) {
        print(LogLevel::Info, "Draw");
        didDraw = true;
    }
}

void shutdown() {
    print(LogLevel::Info, "Shutdown");

}
