#!/bin/bash
#if u dont hv brew install=> /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"


# Check if Homebrew is installed
if ! command -v brew &> /dev/null
then
    echo "Homebrew is not installed. Please install Homebrew and try again.\n"
    exit
fi

# Add MongoDB repository key
brew tap mongodb/brew
brew install gnupg
curl -fsSL https://www.mongodb.org/static/pgp/server-4.4.asc | gpg --import -

# Add MongoDB repository
echo "deb [ arch=amd64 ] https://repo.mongodb.org/apt/macOS/$(uname -m)/archive/$(sw_vers -productName | awk '{print tolower($0)}' | sed 's/ //g')/4.4 main" | sudo tee /etc/apt/sources.list.d/mongodb-org-4.4.list

# Update package manager
brew update

# Install MongoDB C++ driver
brew install mongodb-community

# Verify installation
echo "MongoDB C++ driver installed successfully"
