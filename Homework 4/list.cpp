
void listAll(const Class* c, string path)  // two-parameter overload
{
    path += c->name();
    cout << path << endl;
    for (int k = 0; k < c->subclasses().size(); k++)
    {
        listAll(c->subclasses()[k], path + "=>");
    }
}
