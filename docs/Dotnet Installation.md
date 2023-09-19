# .NET Installation
## Raspberry Pi
Following the [MSDN guide](https://learn.microsoft.com/en-us/dotnet/core/install/linux-scripted-manual) because `apt install` does NOT contain `dotnet-sdk-7.0`,

```sh
sudo apt install libc6 libgcc1 libgssapi-krb5-2 libicu67 libssl1.1 libstdc++6 zlib1g

wget https://dot.net/v1/dotnet-install.sh -O dotnet-install.sh
chmod +x ./dotnet-install.sh

./dotnet-install.sh --version latest
# OR
./dotnet-install.sh --channel 7.0

# In your .bashrc:
export DOTNET_ROOT=$HOME/.dotnet
export PATH=$PATH:$DOTNET_ROOT:$DOTNET_ROOT/tools
```

