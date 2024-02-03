namespace examplelibrary {
    /// @brief An example C++ class, contained in the library to be exported and available to the calling main program that dynamically links against this library.
    class SomeClass {
        private:
            const char* name;
        public:
            SomeClass(const char* name) : name(name) { }
            const char* getName() const { return name; }
            void setName(const char* name) { this->name = name; }
    };
}
