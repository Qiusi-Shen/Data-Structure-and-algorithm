void listAll(const MenuItem* m, string path) // two-parameter overload
{
	//You will write this code.
	if (m->menuItems() == nullptr||m->menuItems()->size()==0)
		return;
	else
	{
		for (vector<MenuItem*>::const_iterator itr = m->menuItems()->begin(); itr != m->menuItems()->end(); itr++)
		{
			cout << path + (*itr)->name() << endl;
			listAll(*itr, path + (*itr)->name() + "/");
		}
	}
}
