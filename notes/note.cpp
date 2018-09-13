Application::Run() {
    // setup stuff

    do {
        for (auto c : m_connections)
            ReadFromSocket(); // async_read
            // Do stuff.
            WriteToSocket(); // async_write
        } 
    } while (m_app_running)
}

// While running.
// -- For Each Connection
// -- -- Handle Input Buffer.
// -- -- Process Command.
// -- -- Output Result.