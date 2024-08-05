# Getting started {#getting-started}

## Creating the Hue bridge
To start searching for a Hue Bridge you will need to choose an IHttpHandler and create one.
The options are a [WinHttpHandler](@ref hueplusplus::WinHttpHandler) (for windows) or a [LinHttpHandler](@ref hueplusplus::LinHttpHandler) (for linux or linux-like).

Then create a [BridgeFinder](@ref hueplusplus::BridgeFinder) object with the handler.
The handler is needed, because it tells the finder which functions to use to communicate with a bridge or your local network.
After that you can call [findBridges()](@ref hueplusplus::BridgeFinder::findBridges), which will return a vector containing the ip and mac address of all found Bridges.
\snippet Snippets.cpp search-bridge

## Authenticate Bridges
If you have found the Bridge you were looking for, you can then move on with the authentication process.
To get a new username from the Bridge (for now) you simply call [getBridge(bridges[\<index\>])](@ref hueplusplus::BridgeFinder::getBridge),
where index is your preferred Bridge from the part [Searching for Bridges](#searchingBridges). This requires the user to press the link button.
\snippet Snippets.cpp get-bridge-1

If you on the other hand already have a username you can add your bridge like so
\snippet Snippets.cpp get-bridge-2

If you do not want to use the BridgeFinder or you already know the ip and username of your bridge you have the option to create your own Hue object.
Here you will need to provide the ip address, the port number, a username and an HttpHandler
\snippet Snippets.cpp get-bridge-3

At this point you may want to decide whether to use a [shared state](@ref shared-state) cache model or keep the default settings.

### Controlling lights

\snippet Snippets.cpp control-lights

Use [transactions](@ref transactions) to change multiple properties at once.

### Controlling groups

\snippet Snippets.cpp control-groups

## More information
- [Transactions](@ref transactions)
- [Shared state cache](@ref shared-state)
