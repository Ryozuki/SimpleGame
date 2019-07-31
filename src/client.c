#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <SDL_net.h>
#include "color.h"
#include "list.h"
#include "protocol.h"

static SDL_Window *pWindow = NULL;
static SDL_Renderer *pRenderer = NULL;
static UDPsocket sock;
static int port = 6666;
static int channel;
static UDPpacket *packet = NULL;
static UDPpacket *sendPacket = NULL;

int send_packet(int clean) {
	int sent;
	sent = SDLNet_UDP_Send(sock, channel, sendPacket);
	if(!sent) {
		fprintf(stderr, "Error sending packet: %s\n", SDLNet_GetError());
	}
	if(clean == 1) {
		memset(sendPacket->data, 0, PACKET_SIZE);
	}
	return sent;
}

int main(int argc, const char* argv[]) {
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) == -1) {
		fprintf(stderr, "Error initializing SDL2: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	pWindow = SDL_CreateWindow("SimpleGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_SHOWN);

	if(!pWindow) {
		fprintf(stderr, "Error creating window: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);

	if(!pRenderer) {
		fprintf(stderr, "Error creating renderer: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}


	if(SDLNet_Init() == -1) {
		fprintf(stderr, "Error initializing SDLNet: %s\n", SDLNet_GetError());
		return EXIT_FAILURE;
	}

	sock = SDLNet_UDP_Open(0);

	if(!sock) {
		fprintf(stderr, "Error opening socket at port %d: %s\n", 6666, SDLNet_GetError());
		return EXIT_FAILURE;
	}

	IPaddress address;

	SDLNet_ResolveHost(&address, "localhost", port);

	channel = SDLNet_UDP_Bind(sock, -1, &address);

	if(channel == -1) {
		fprintf(stderr, "Error binding socket: %s\n", SDLNet_GetError());
		return EXIT_FAILURE;
	}

	UDPpacket *packet;
	UDPpacket *sendPacket;

	packet = SDLNet_AllocPacket(PACKET_SIZE);
	sendPacket = SDLNet_AllocPacket(PACKET_SIZE);
	memset(packet->data, 0, PACKET_SIZE);
	memset(sendPacket->data, 0, PACKET_SIZE);

	int nrecv;

	StrList *list = strlist_create();

	while(1) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				goto cleanup;
			}
		}

		// Update

		nrecv = SDLNet_UDP_Recv(sock, packet);
		if(nrecv == -1) {
			fprintf(stderr, "Error receiving a packet: %s\n", SDLNet_GetError());
		}

		if(nrecv > 0) {
			printf(COLOR_GREEN "Received packet with size: %d\n" COLOR_RESET, nrecv);
		}

		// Render
		SDL_SetRenderDrawColor(pRenderer, 46, 46, 46, 255);
		SDL_RenderClear(pRenderer);

		int y = 20;

		StrListValue *cur = list->head;

		while(cur != NULL) {
			stringRGBA(pRenderer, 20, y, cur->value, 200, 0, 0, 255);
			cur = cur->next;
			y += 15;
		}

		SDL_RenderPresent(pRenderer);
		SDL_Delay(1);
	}

cleanup:
	strlist_free(list);
	SDLNet_FreePacket(packet);
	SDLNet_FreePacket(sendPacket);
	SDLNet_UDP_Unbind(sock, channel);
	SDLNet_UDP_Close(sock);
	SDL_DestroyRenderer(pRenderer);
	SDL_DestroyWindow(pWindow);
	SDLNet_Quit();
	SDL_Quit();
	return EXIT_SUCCESS;
}
