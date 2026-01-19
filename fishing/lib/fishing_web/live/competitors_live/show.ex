defmodule FishingWeb.CompetitorsLive.Show do
  use FishingWeb, :live_view

  alias Fishing.Registration

  @impl true
  def mount(_params, _session, socket) do
    {:ok, socket}
  end

  @impl true
  def handle_params(%{"id" => id}, _, socket) do
    {:noreply,
     socket
     |> assign(:page_title, page_title(socket.assigns.live_action))
     |> assign(:competitors, Registration.get_competitors!(id))}
  end

  defp page_title(:show), do: "Show Competitors"
  defp page_title(:edit), do: "Edit Competitors"
end
