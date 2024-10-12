# Spotify.Tracker 

> That's not officall, that's a personal use only project.

### What's Coming?
- [x] Use spotify api instead of cmd
- [x] Spotify_app requests/responses
- [x] Server_side recieve requests/send responses
- [x] Fixing memory leaks and bugs

### NOTE
Pls, create an issue if there's any bugs.

## USAGE

### Terminal
Open your spotify on web/mobile/app and run this command on terminal (don't forget to be in the correct folder)
```bash
./Spotify.Tracker
```

### Spotify Developer Dashboard
- You should create [your own developer account](https://developer.spotify.com/dashboard).
- Create your app as a developer.
- Add redirect urls.
- - http://localhost:3131/api
- - http://localhost:3131/api/access-token
- - http://localhost:3131/api/authorization-code
- Add your Client Id and Client Secret to your own app_settings.json (When you run program, it'll be created automatically in settings folder.)
- Authorize the access request when you run program.

### Spotify Design
- Don't forget to follow [the guidlines](https://developer.spotify.com/documentation/design).
- Send request to http://localhost:3131/api/current-track (I send request to here in every 3 seconds)

### My Design 
![Screenshot 2024-10-13 at 2 50 14 am](https://github.com/user-attachments/assets/f6fdde30-1abe-481f-8b6f-17c6b992c56c)
