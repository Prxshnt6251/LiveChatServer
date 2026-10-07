import { useState, useEffect, useRef } from 'react';
import './index.css';

function App() {
  const [joined, setJoined] = useState(false);
  const [username, setUsername] = useState('');
  const [room, setRoom] = useState('general');
  const [messages, setMessages] = useState([]);
  const [inputText, setInputText] = useState('');
  const [connected, setConnected] = useState(false);
  const ws = useRef(null);
  const messagesEndRef = useRef(null);

  const scrollToBottom = () => {
    messagesEndRef.current?.scrollIntoView({ behavior: 'smooth' });
  };

  useEffect(() => {
    scrollToBottom();
  }, [messages]);

  useEffect(() => {
    return () => {
      if (ws.current) {
        ws.current.close();
      }
    };
  }, []);

  const handleJoin = (e) => {
    e.preventDefault();
    if (!username.trim() || !room.trim()) return;

    ws.current = new WebSocket('ws://localhost:8080');

    ws.current.onopen = () => {
      setConnected(true);
      setJoined(true);
      
      const joinMsg = {
        type: 'join',
        room: room,
        user: username
      };
      ws.current.send(JSON.stringify(joinMsg));
      
      setMessages([{
        type: 'system',
        text: `Joined room: ${room}`
      }]);
    };

    ws.current.onmessage = (event) => {
      try {
        const data = JSON.parse(event.data);
        if (data.type === 'msg') {
          setMessages(prev => [...prev, data]);
        }
      } catch (err) {
        console.error('Error parsing message', err);
      }
    };

    ws.current.onclose = () => {
      setConnected(false);
      setMessages(prev => [...prev, {
        type: 'system',
        text: 'Disconnected from server.'
      }]);
    };
  };

  const handleSend = (e) => {
    e.preventDefault();
    if (!inputText.trim() || !ws.current || ws.current.readyState !== WebSocket.OPEN) return;

    const msg = {
      type: 'msg',
      room: room,
      user: username,
      text: inputText
    };

    ws.current.send(JSON.stringify(msg));
    setInputText('');
  };

  return (
    <div className="app-container">
      <header className="header">
        <h1>LiveChat</h1>
        <div className="status">
          <div className={`status-dot ${connected ? 'connected' : ''}`}></div>
          {connected ? `Connected (${room})` : 'Disconnected'}
        </div>
      </header>

      {!joined ? (
        <form className="login-container" onSubmit={handleJoin}>
          <h2>Join a Room</h2>
          <div className="input-group">
            <label>Username</label>
            <input 
              type="text" 
              value={username}
              onChange={e => setUsername(e.target.value)}
              placeholder="Enter your name"
              required
            />
          </div>
          <div className="input-group">
            <label>Room</label>
            <input 
              type="text" 
              value={room}
              onChange={e => setRoom(e.target.value)}
              placeholder="e.g. general"
              required
            />
          </div>
          <button type="submit">Join Chat</button>
        </form>
      ) : (
        <div className="chat-area">
          <div className="messages">
            {messages.map((msg, idx) => (
              <div 
                key={idx} 
                className={`message ${msg.type === 'system' ? 'system' : (msg.user === username ? 'self' : 'other')}`}
              >
                {msg.type !== 'system' && (
                  <div className="message-header">
                    {msg.user}
                  </div>
                )}
                <div className="message-bubble">
                  {msg.text}
                </div>
              </div>
            ))}
            <div ref={messagesEndRef} />
          </div>
          <form className="input-area" onSubmit={handleSend}>
            <input 
              type="text"
              value={inputText}
              onChange={e => setInputText(e.target.value)}
              placeholder="Type a message..."
              autoFocus
            />
            <button type="submit">Send</button>
          </form>
        </div>
      )}
    </div>
  );
}

export default App;
