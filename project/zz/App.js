import SignInPage from "./pages/SignIn"
import RegisterPage from "./pages/Register";
import NoteCard from "./components/NoteCard";

function App() {
  const categories = ["Educational", "Learn", "New", "Science", "Youtube", "idk"]
  return (
    <div className="flex justify-center items-center w-screen h-screen">
        <NoteCard 
        title="My Title" 
        text="This can be a note preview or note description, whichever works best"
        categories={categories}
        lastEdited="Edited 16 Sep 2023"/>
    </div>
    );
}

export default App;
